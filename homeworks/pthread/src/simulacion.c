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
#include <pthread.h>
#include <controlador.h>

int cambio_temperatura(double* temperaturas, Plate* plate,
                                                 shared_data_t* shared_data) {
    int iteraciones = 0;

    // Crear memoria para hilos
    pthread_t* hilos = malloc(shared_data->cantidadHilos * sizeof(pthread_t));
    if (hilos == NULL) {
        printf("Error: No se pudo asignar memoria para los hilos.\n");
        return -1;
    }

    // Crear memoria para private_data
    private_data_t* private_data = calloc(shared_data->cantidadHilos,
                                                     sizeof(private_data_t));
    if (private_data == NULL) {
        printf("Error: No se pudo asignar memoria para datos privados.\n");
        free(hilos);
        return -1;
    }

    // Asignar datos comunes
    dividir_array(private_data, shared_data, *plate);
    for (int i = 0; i < shared_data->cantidadHilos; i++) {
        private_data[i].plate = plate;
        private_data[i].temperaturas = temperaturas;
        private_data[i].shared_data = *shared_data;
    }

    // Bucle de simulación
    double* matriz_temporal = malloc(plate->R * plate->C * sizeof(double));
    if (matriz_temporal == NULL) {
        printf("Error: No se pudo asignar memoria para la matriz temporal.\n");
        free(hilos);
        free(private_data);
        return -1;
    }

    do {
        iteraciones++;

        // Copiar temperaturas actuales a la matriz temporal
        memcpy(matriz_temporal, temperaturas, plate->R * plate->C *
                                                             sizeof(double));

        // Asignar puntero a la matriz temporal
        for (int i = 0; i < shared_data->cantidadHilos; i++) {
            private_data[i].temperaturas_temporal = matriz_temporal;
        }

        // Crear hilos
        for (int i = 0; i < shared_data->cantidadHilos; i++) {
            if (pthread_create(&hilos[i], NULL, cambio_temperatura_hilos,
                                                     &private_data[i]) != 0) {
                printf("Error: No se pudo crear el hilo %d\n", i); //NOLINT
                free(hilos);
                free(private_data);
                free(matriz_temporal);
                return -1;
            }
        }

        shared_data->cambio_maximo_global = 0.0;

        // Esperar hilos
        for (int i = 0; i < shared_data->cantidadHilos; i++) {
            pthread_join(hilos[i], NULL);
            if (private_data[i].cambio_maximo_local >
                                         shared_data->cambio_maximo_global) {
                shared_data->cambio_maximo_global =
                                            private_data[i].cambio_maximo_local;
            }
        }

        // Actualizar la matriz original
        memcpy(temperaturas, matriz_temporal, plate->R * plate->C *
                                                             sizeof(double));
    } while (shared_data->cambio_maximo_global >= plate->epsilon);

    // Liberar recursos
    free(matriz_temporal);
    free(hilos);
    free(private_data);

    // Asignar resultados al plate
    plate->iteraciones = iteraciones;
    plate->tiempoSegundos = iteraciones * plate->delta;

    return iteraciones;
}

void* cambio_temperatura_hilos(void* arg) {
    private_data_t* private = (private_data_t*) arg;
    Plate* plate = private->plate;
    double* temp = private->temperaturas;
    double* temp_local = private->temperaturas_temporal;
    double cambio_maximo_temporal = 0.0;

    uint64_t inicio = private->inicio;
    uint64_t final = private->final;
    uint64_t columnas = plate->C;

    for (uint64_t idx = inicio; idx < final; idx++) {
        // Fila correspondiente
        uint64_t i = idx / columnas;
        // Columna correspondiente
        uint64_t j = idx % columnas;   

        if (i == 0 || i == plate->R - 1 || j == 0 || j == plate->C - 1) {
            temp_local[idx] = temp[idx];
            // los bordes no cambian, se copian directamente

        } else {
            double arriba = temp[(i - 1) * columnas + j];
            double abajo = temp[(i + 1) * columnas + j];
            double izq = temp[i * columnas + (j - 1)];
            double der = temp[i * columnas + (j + 1)];

            temp_local[idx] = temp[idx] + plate->alfa * plate->delta *
                ((arriba + abajo + izq + der - 4.0 * temp[idx]) /
                (plate->h * plate->h));
        }
        double cambio = fabs(private->temperaturas_temporal[idx] -
                                                 private->temperaturas[idx]);

            if (cambio > cambio_maximo_temporal) {
                cambio_maximo_temporal = cambio;
            }
    }
    private->cambio_maximo_local = cambio_maximo_temporal;
    return NULL;
}
