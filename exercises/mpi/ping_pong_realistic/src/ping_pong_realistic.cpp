// Copyright [2025] <Liqing Yosery Zheng Lu>

#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <random>

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

    // Verificar argumentos
    if (argc != 4) {
        std::cerr << "Error: Este programa requiere 3 argumentos: marcador de victoria, probabilidad jugador 1, probabilidad jugador 2." << std::endl;
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    // Marcador de victoria
    int victory_score = std::atoi(argv[1]);

    // Probabilidades de acierto de los jugadores
    double accuracy_player_1 = std::atof(argv[2]);
    double accuracy_player_2 = std::atof(argv[3]);

    // Marcadores de victoria de cada jugador
    int score_player_1 = 0;
    int score_player_2 = 0;

    int bola = 0;

    // Rondas jugadas
    int round = 0;

    // Generador de números aleatorios para simular aciertos
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 100.0);

    while (score_player_1 < victory_score && score_player_2 < victory_score) {
        round++;
        if (process_rank == 0) {

            // Simular acierto del jugador 1
            double random_value = dist(gen);
            bool hit = random_value <= accuracy_player_1;

            if (hit) {
                bola = 1; // Jugador 1 gana la ronda
                score_player_1++;
            } else {
                bola = 0; // Jugador 2 gana la ronda
                score_player_2++;
            }

            // Proceso 0 envía la bola al proceso 1
            std::cout << process_rank << " serves" << std::endl;
            MPI_Send(&bola, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

            // Imprimir resultado de la ronda
            std::cout << round << ": " << random_value << " " << bola << std::endl;

            // Proceso 0 recibe la bola de vuelta con etiqueta 1
            MPI_Recv(&bola, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        } else if (process_rank == 1) {
            // Proceso 1 espera la bola del proceso 0
            MPI_Recv(&bola, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //NOLINT

            // Simular acierto del jugador 2
            double random_value = dist(gen);
            bool hit = random_value <= accuracy_player_2;

            if (hit) {
                bola = 0; // Jugador 2 gana la ronda
                score_player_2++;
            } else {
                bola = 1; // Jugador 1 gana la ronda
                score_player_1++;
            }

            // Proceso 1 envía la bola de vuelta al proceso 0
            std::cout << process_rank << " serves" << std::endl;
            MPI_Send(&bola, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);

            // Imprimir resultado de la ronda
            std::cout << round << ": " << random_value << " " << bola << std::endl;
        }
    }

    // Imprimir resultado final
    if (process_rank == 0) {
        if (score_player_1 > score_player_2) {
            std::cout << "0 wins " << score_player_1 << " to " << score_player_2 << std::endl;
        } else if (score_player_2 > score_player_1) {
            std::cout << "1 wins " << score_player_2 << " to " << score_player_1 << std::endl;
        } else {
            std::cout << "It's a tie " << score_player_1 << " to " << score_player_2 << std::endl;
        }
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}
