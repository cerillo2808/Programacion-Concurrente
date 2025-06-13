// Copyright [2025] <Liqing Yosery Zheng Lu>

#include <stdio.h>
#include <stdlib.h>
#include <controlador.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank;
    
    // Inicializar MPI para poder obtener el rank
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    // Ejecutar la simulación
    int result = run_mpi(argc, argv);
    
    // Solo el proceso 0 imprime el mensaje final
    if (rank == 0) {
        if (result == 0) {
            printf("Simulación completada exitosamente\n");
        } else {
            printf("Error en la simulación\n");
        }
    }
    
    // MPI_Finalize ya se llama en run_mpi, pero por si acaso
    // verificamos si MPI ya fue finalizado
    int finalized;
    MPI_Finalized(&finalized);
    if (!finalized) {
        MPI_Finalize();
    }
    
    return result;
}