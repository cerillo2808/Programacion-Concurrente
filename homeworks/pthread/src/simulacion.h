// Copyright [2025] <Liqing Yosery Zheng Lu>

#ifndef SIMULACION_H
#define SIMULACION_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

// declaro los structs que existen en controlador.h
typedef struct shared_data shared_data_t;
typedef struct private_data private_data_t;

/**
 * @brief Ejecuta la simulación de difusión térmica en una lámina utilizando
 * múltiples hilos, hasta que el cambio máximo entre iteraciones sea menor
 * que el umbral epsilon.
 *
 * @param temperaturas Puntero a la matriz de temperaturas de la lámina. Este
 * arreglo se actualizará con los nuevos valores tras la simulación.
 *
 * @param plate Puntero a la estructura Plate, que contiene los parámetros
 * físicos de la simulación como dimensiones, epsilon, y delta de tiempo.
 *
 * @param shared_data Puntero a la estructura shared_data_t, que contiene
 * datos compartidos como la cantidad de hilos y el cambio máximo global.
 *
 * @return El número total de iteraciones realizadas durante la simulación.
 * Retorna -1 si ocurre un error al asignar memoria o crear hilos.
 */
int cambio_temperatura(double* temperaturas, Plate* plate,
                                                   shared_data_t* shared_data);

/**
 * @brief Función ejecutada por cada hilo para calcular la difusión térmica
 * en una sección de la lámina asignada.
 *
 * Cada hilo procesa un subconjunto de celdas de la matriz de temperaturas.
 * Las celdas de borde se copian sin cambios, mientras que las internas se
 * actualizan según el modelo de difusión. Al finalizar, se guarda el cambio
 * máximo local detectado por el hilo.
 *
 * @param arg Puntero a una estructura private_data_t que contiene los datos
 * necesarios para la simulación de este hilo, incluyendo los índices de
 * inicio y fin, las matrices de temperatura y los parámetros físicos.
 *
 * @return Siempre retorna NULL. El resultado se almacena en la estructura
 * private_data_t proporcionada.
 */
void* cambio_temperatura_hilos(void* arg);

#endif
