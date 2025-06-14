// Copyright [2025] <Liqing Yosery Zheng Lu>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "controlador.h"
#include "plate.h"
#include "simulacion.h"
#include "escritor_archivos.h"

#define TAG_LINEA 100
#define TAG_FIN 200

void iniciar_simulacion(char *argumentos[]) {
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  FILE* archivo = fopen(argumentos[1], "r");
  if (!archivo) {
    printf("Error al abrir el archivo de trabajo.\n");
    return;
  }

  // Contar cuántas líneas hay en el archivo
  char lineas[1024][512];
  int total_lineas = 0;
  while (fgets(lineas[total_lineas], sizeof(lineas[total_lineas]), archivo)) {
    total_lineas++;
  }
  fclose(archivo);

  // Inicializar cantidad de líneas que hay y cantidad de líneas que se envían
  int enviados = 0, recibidos = 0;
  for (int i = 1; i < world_size && enviados < total_lineas; ++i) {
    MPI_Send(lineas[enviados], (int)strlen(lineas[enviados])+1,
                       MPI_CHAR, i, TAG_LINEA, MPI_COMM_WORLD);
    enviados++;
  }

  // El proceso maestro recibe las líneas de los trabajadores
  // Envía líneas hasta que se hayan enviado todas
  while (recibidos < total_lineas) {
    // Espera a que un trabajador termine su tarea
    // Envía una nueva línea si hay disponible
    // Repartición dinámica de trabajo
    MPI_Status status;
    MPI_Recv(NULL, 0, MPI_CHAR, MPI_ANY_SOURCE, TAG_FIN,
                               MPI_COMM_WORLD, &status);
    int worker = status.MPI_SOURCE;
    // Cuando un trabajador termina, se incrementa el contador de recibidos
    recibidos++;

    if (enviados < total_lineas) {
      // Si hay más trabajadores que líneas, cada trabajador recibe una línea
      // Los otros trabajadores no reciben nada
      MPI_Send(lineas[enviados], (int)strlen(lineas[enviados])+1,
                     MPI_CHAR, worker, TAG_LINEA, MPI_COMM_WORLD);
      // Se incrementa el contador de enviados
      enviados++;
    } else {
      for (int i = 1; i < world_size; ++i) {
        // Si ya no hay más líneas para enviar
        // Se envía un mensaje de fin a todos los trabajadores
        MPI_Send(NULL, 0, MPI_CHAR, i, TAG_FIN, MPI_COMM_WORLD);
      }
    }
  }
}

void worker_simulacion(char *argumentos[]) {
  char linea[512];
  MPI_Status status;

  while (1) {
    // Recibe una línea del proceso maestro
    // Si recibe un mensaje de fin, sale del bucle
    MPI_Recv(linea, sizeof(linea), MPI_CHAR, 0,
           MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    if (status.MPI_TAG == TAG_FIN) {
      break;
    }

    // Concatena el nombre del archivo con el nombre del job
    char job_nombre[256];
    char* base = strrchr(argumentos[1], '/');
    base = base ? base + 1 : argumentos[1];
    strncpy(job_nombre, base, sizeof(job_nombre));
    job_nombre[sizeof(job_nombre)-1] = '\0';
    char *punto = strrchr(job_nombre, '.');
    if (punto != NULL) *punto = '\0';
    strncat(job_nombre, ".tsv", sizeof(job_nombre) - strlen(job_nombre) - 1);

    Plate plate = crear_plate(linea);

    double* temperaturas = leer_plate(plate.nombreArchivo,
                                   &plate, argumentos[2]);
    cambio_temperatura(temperaturas, &plate, 1);
    nombreBin(&plate);
    generar_archivo_binario(plate.nombreBin, plate.R, plate.C,
                                                          temperaturas);
    nombreTsv(&plate);
    generar_archivo_tsv("output", job_nombre, plate,
                              plate.tiempoSegundos, plate.iteraciones);

    free(temperaturas);
    // Envía un mensaje de fin al proceso maestro
    MPI_Send(NULL, 0, MPI_CHAR, 0, TAG_FIN, MPI_COMM_WORLD);
  }
}
