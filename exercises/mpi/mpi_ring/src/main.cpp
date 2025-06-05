#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int process_count = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);

    // Verificar que sean exactamente 4 procesos
    if (process_count != 4) {
        std::cerr << "Error: Este programa requiere exactamente 4 procesos, pero se lanzó con " << process_count << " procesos." << std::endl;
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    int process_rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    // Generar un número aleatorio entre [0, 100[
    std::random_device rd; // Generador de números aleatorios basado en hardware
    std::mt19937 gen(rd()); // Generador Mersenne Twister
    std::uniform_int_distribution<> dist(0, 99); // Distribución uniforme en el rango [0, 99]

    int random_number = dist(gen);

    std::cout << "Proceso " << process_rank << " generó el número aleatorio: " << random_number << std::endl;

    // Comunicación circular para calcular la suma total
    int sum = 0;
    int numero_random = random_number;

    for (int i = 1; i < process_count; ++i) {
        int next_process = (process_rank + 1) % process_count; // Proceso siguiente
        int prev_process = (process_rank - 1 + process_count) % process_count; // Proceso anterior

        if (process_rank == 0) {
            // Actualizar la suma acumulada
            sum += numero_random;
            // Enviar la suma acumulada al siguiente proceso
            MPI_Send(&sum, 1, MPI_INT, next_process, 0, MPI_COMM_WORLD);
            // Recibir la suma acumulada del proceso anterior
            MPI_Recv(&sum, 1, MPI_INT, prev_process, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "Proceso " << process_rank << " recibe " << sum << std::endl;
            

        } else {
            // Recibir la suma acumulada del proceso anterior
            MPI_Recv(&sum, 1, MPI_INT, prev_process, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "Proceso " << process_rank << " recibe " << sum << std::endl;
            if (i == 1){
                // Actualizar la suma acumulada
            sum += numero_random;
            }
            // Enviar la suma acumulada al siguiente proceso
            MPI_Send(&sum, 1, MPI_INT, next_process, 0, MPI_COMM_WORLD);
        }
    }

    // Imprimir la suma total
    std::cout << "Proceso " << process_rank << " tiene la suma total: " << sum << std::endl;
    MPI_Finalize();
    return EXIT_SUCCESS;
}