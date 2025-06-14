// Copyright [2025] <Liqing Yosery Zheng Lu>

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "controlador.h"

int main(int argc, char *argv[]) {
  // Inicializar MPI
  MPI_Init(&argc, &argv);
  int rank;
  // Obtener el rango del proceso
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (argc < 3) {
    if (rank == 0) {
    // Imprimir mensaje de uso si no se proporcionan argumentos
      printf("Uso: %s job.txt directorio\n", argv[0]);
    }
    // Finalizar MPI y salir si no se proporcionan argumentos
    MPI_Finalize();
    return 1;
  }
  if (rank == 0) {
    // Si es el proceso maestro, iniciar la simulación
    iniciar_simulacion(argv);
  } else {
    // Si es un proceso trabajador, ejecutar la simulación
    worker_simulacion(argv);
  }
  // Finalizar MPI
  MPI_Finalize();
  return 0;
}
