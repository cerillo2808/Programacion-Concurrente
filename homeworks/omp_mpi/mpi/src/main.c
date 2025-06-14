
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "controlador.h"

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (argc < 3) {
    if (rank == 0)
      printf("Uso: %s job.txt directorio\n", argv[0]);
    MPI_Finalize();
    return 1;
  }
  if (rank == 0) {
    iniciar_simulacion(argv);
  } else {
    worker_simulacion(argv);
  }
  MPI_Finalize();
  return 0;
}