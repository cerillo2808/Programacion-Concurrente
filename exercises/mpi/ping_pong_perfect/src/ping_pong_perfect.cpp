// Copyright [2025] <Liqing Yosery Zheng Lu>

#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int process_count = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);
    if (process_count != 2) {
        std::cerr << "Error: Este programa requiere exactamente 2 procesos"
        << ", pero se lanzó con " << process_count << " procesos." << std::endl;
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    int process_rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    // Verificar tiempo
    int wait_time = 0;
    if (argc == 3) {
        wait_time = std::atoi(argv[2]);
    }

    int bola = 0;
    // Los jugadores son incansables
    while (true) {
        if (process_rank == 0) {
            // Proceso 0 envía la bola al proceso 1
            std::cout << process_rank << " serves" << std::endl;
            MPI_Send(&bola, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

            // Espera la bola de vuelta del proceso 1
            MPI_Recv(&bola, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //NOLINT

            // Delay opcional
            if (wait_time > 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(wait_time)); //NOLINT
            }

        } else if (process_rank == 1) {
            // Proceso 1 espera la bola del proceso 0
            MPI_Recv(&bola, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //NOLINT

            // Proceso 1 envía la bola de vuelta al proceso 0
            std::cout << process_rank << " serves" << std::endl;
            MPI_Send(&bola, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

            // Delay opcional
            if (wait_time > 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(wait_time)); //NOLINT
            }
        }
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}
