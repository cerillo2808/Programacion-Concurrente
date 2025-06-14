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

  char lineas[1024][512];
  int total_lineas = 0;
  while (fgets(lineas[total_lineas], sizeof(lineas[total_lineas]), archivo)) {
    total_lineas++;
  }
  fclose(archivo);

  int enviados = 0, recibidos = 0;
  for (int i = 1; i < world_size && enviados < total_lineas; ++i) {
    MPI_Send(lineas[enviados], (int)strlen(lineas[enviados])+1, MPI_CHAR, i, TAG_LINEA, MPI_COMM_WORLD);
    enviados++;
  }

  while (recibidos < total_lineas) {
    MPI_Status status;
    MPI_Recv(NULL, 0, MPI_CHAR, MPI_ANY_SOURCE, TAG_FIN, MPI_COMM_WORLD, &status);
    int worker = status.MPI_SOURCE;
    recibidos++;

    if (enviados < total_lineas) {
      MPI_Send(lineas[enviados], (int)strlen(lineas[enviados])+1, MPI_CHAR, worker, TAG_LINEA, MPI_COMM_WORLD);
      enviados++;
    } else {
      MPI_Send(NULL, 0, MPI_CHAR, worker, TAG_LINEA, MPI_COMM_WORLD);
    }
  }
}

void worker_simulacion(char *argumentos[]) {
  char linea[512];
  MPI_Status status;

  char job_nombre[256];
  char* base = strrchr(argumentos[1], '/');
  base = base ? base + 1 : argumentos[1];
  strncpy(job_nombre, base, sizeof(job_nombre));
  job_nombre[sizeof(job_nombre)-1] = '\0';
  char *punto = strrchr(job_nombre, '.');
  if (punto != NULL) *punto = '\0';

  while (1) {
    MPI_Recv(linea, sizeof(linea), MPI_CHAR, 0, TAG_LINEA, MPI_COMM_WORLD, &status);
    if (status._ucount == 0) break;

    Plate plate = crear_plate(linea);
    strncpy(plate.nombreJob, job_nombre, sizeof(plate.nombreJob));
    plate.nombreJob[sizeof(plate.nombreJob)-1] = '\0';

    double* temperaturas = leer_plate(plate.nombreArchivo, &plate, argumentos[2]);
    cambio_temperatura(temperaturas, &plate, 1);

    generar_archivo_tsv(plate.nombreArchivo, job_nombre, plate, plate.tiempoSegundos, plate.iteraciones);
    generar_archivo_binario(plate.nombreBin, plate.R, plate.C, temperaturas);

    free(temperaturas);
    MPI_Send(NULL, 0, MPI_CHAR, 0, TAG_FIN, MPI_COMM_WORLD);
  }
}
