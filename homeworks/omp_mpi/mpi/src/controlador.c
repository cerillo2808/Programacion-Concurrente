
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "controlador.h"
#include "plate.h"
#include "simulacion.h"
#include "escritor_archivos.h"

#define TAG_LINEA 100

void iniciar_simulacion(char *argumentos[]) {
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  FILE* archivo = fopen(argumentos[1], "r");
  if (!archivo) {
    printf("Error al abrir el archivo de trabajo.\n");
    return;
  }

  char linea[512];
  int worker_id = 1;
  while (fgets(linea, sizeof(linea), archivo)) {
    MPI_Send(linea, (int)strlen(linea)+1, MPI_CHAR, worker_id, TAG_LINEA, MPI_COMM_WORLD);
    worker_id++;
    if (worker_id >= world_size) break;
  }

  for (int i = worker_id; i < world_size; ++i) {
    MPI_Send("FIN", 4, MPI_CHAR, i, TAG_LINEA, MPI_COMM_WORLD);
  }

  fclose(archivo);
}

void worker_simulacion(char *argumentos[]) {
  char linea[512];
  MPI_Recv(linea, sizeof(linea), MPI_CHAR, 0, TAG_LINEA, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  if (strcmp(linea, "FIN") == 0) return;

  Plate plate = crear_plate(linea);
  double* temperaturas = leer_plate(plate.nombreArchivo, &plate, argumentos[2]);

  cambio_temperatura(temperaturas, &plate, 1);

  // Corrección de path
  char pathTSV[512];
  snprintf(pathTSV, sizeof(pathTSV), "%s/%s", argumentos[2], plate.nombreJob);
  generar_archivo_tsv(plate.nombreArchivo, pathTSV, plate, plate.tiempoSegundos, plate.iteraciones);

  free(temperaturas);
}