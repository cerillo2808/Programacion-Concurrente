// Copyright [2025] <Liqing Yosery Zheng Lu>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <plate.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <math.h>
#include <simulacion.h>

void cambio_temperatura(double* temperaturas, Plate* plate, int particiones) {
    int iteraciones = 0;
    double cambio_maximo = 0;
    double cambio = 0;

    // Crear matriz temporal para calcular el cambio
    double *temperaturas_temporal = (double *)malloc(plate->R * plate->C * sizeof(double)); //NOLINT
    if (!temperaturas_temporal) {
        printf("Error: No se pudo asignar memoria para la matriz temporal.\n"); //NOLINT
        return;
    }

    // Calcular el tamaño de cada partición
    uint64_t total_celdas = plate->R * plate->C;
    uint64_t celdas_por_particion = total_celdas / particiones;

    do {
        iteraciones++;
        cambio_maximo = 0.0;

         // Copiar filas de borde
        for (size_t j = 0; j < plate->C; j++) {
            temperaturas_temporal[0 * plate->C + j] =
                                                temperaturas[0 * plate->C + j];
            temperaturas_temporal[(plate->R - 1) * plate->C + j] =
                                    temperaturas[(plate->R - 1) * plate->C + j];
        }

        // Copiar columnas de borde
        for (size_t i = 1; i < plate->R - 1; i++) {
            temperaturas_temporal[i * plate->C + 0] =
                                                temperaturas[i * plate->C + 0];
            temperaturas_temporal[i * plate->C + (plate->C - 1)]
            = temperaturas[i * plate->C + (plate->C - 1)];
        }

        for (int p = 0; p < particiones; p++) {
            uint64_t inicio = p * celdas_por_particion;
            uint64_t fin = (p == particiones - 1) ? total_celdas : inicio + celdas_por_particion; //NOLINT

            for (uint64_t idx = inicio; idx < fin; idx++) {
                uint64_t i = idx / plate->C;
                uint64_t j = idx % plate->C;

                if (i == 0 || i == plate->R - 1 || j == 0 || j == plate->C - 1) { //NOLINT
                    temperaturas_temporal[idx] = temperaturas[idx];
                } else {
                    double arriba = temperaturas[(i - 1) * plate->C + j];
                    double abajo = temperaturas[(i + 1) * plate->C + j];
                    double izquierda = temperaturas[i * plate->C + (j - 1)];
                    double derecha = temperaturas[i * plate->C + (j + 1)];

                    temperaturas_temporal[idx] = temperaturas[idx] +
                        plate->alfa * plate->delta *
                        ((arriba + abajo + izquierda + derecha - 4.0 * temperaturas[idx]) / (plate->h * plate->h)); //NOLINT

                    cambio = fabs(temperaturas_temporal[idx] - temperaturas[idx]); //NOLINT
                    if (cambio > cambio_maximo) {
                        cambio_maximo = cambio;
                    }
                }
            }
        }

        memcpy(temperaturas, temperaturas_temporal, plate->R * plate->C * sizeof(double)); //NOLINT
    } while (cambio_maximo > plate->epsilon);

    plate->iteraciones = iteraciones;
    plate->tiempoSegundos = iteraciones * plate->delta;

    free(temperaturas_temporal);
}
