// Copyright [2025] <Liqing Yosery Zheng Lu>

#ifndef SIMULACION_MPI_H
#define SIMULACION_MPI_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <plate.h>
#include <mpi.h>

// Forward declaration para evitar dependencia circular
typedef struct shared_data_mpi shared_data_mpi_t;
typedef struct private_data_mpi private_data_mpi_t;

/**
 @brief Realiza la simulación de cambio de temperatura en una placa utilizando
  MPI para la paralelización
 @param temperaturas Arreglo de temperaturas actuales en la placa
 @param plate Estructura que contiene los parámetros de la placa, como
  dimensiones y constantes de simulación
 @param shared_data Puntero a la estructura de datos compartidos MPI
 
 Esta función realiza iteraciones de simulación para calcular el cambio de
 temperatura en cada punto de la placa, utilizando un método de diferencias
 finitas distribuido con MPI. La simulación se repite hasta que el cambio 
 máximo global entre las iteraciones es menor que el valor de epsilon 
 especificado en la estructura plate.

 @return No tiene valor de retorno. Los resultados se actualizan en el arreglo
 de temperaturas y en la estructura plate.
*/
void cambio_temperatura_mpi(double* temperaturas, Plate* plate, shared_data_mpi_t* shared_data);

/**
 @brief Calcula una iteración de la simulación térmica para la porción
  asignada a un proceso MPI
 @param private_data Puntero a la estructura de datos privados del proceso
  que contiene su rango de trabajo y matrices temporales

 Esta función ejecuta los cálculos de diferencias finitas para las celdas
 asignadas al proceso actual. Aplica las condiciones de borde y calcula el
 cambio máximo local encontrado en esta iteración.

 @return No tiene valor de retorno. Los resultados se almacenan en la matriz
 temporal y el cambio máximo local se actualiza en private_data.
*/
void calcular_iteracion_mpi(private_data_mpi_t* private_data);

/**
 @brief Reúne las temperaturas calculadas por todos los procesos MPI
 @param temperaturas_temporal Arreglo temporal donde se almacenarán todas
  las temperaturas reunidas
 @param plate Estructura que contiene las dimensiones de la placa
 @param shared_data Puntero a la estructura de datos compartidos MPI

 Esta función utiliza MPI_Gatherv para reunir las porciones de la matriz de
 temperaturas calculadas por cada proceso en el proceso 0. Maneja la
 distribución no uniforme cuando el número de elementos no es divisible
 exactamente entre los procesos.

 @return No tiene valor de retorno. Las temperaturas se reúnen en el proceso 0.
*/
void reunir_temperaturas_mpi(double* temperaturas_temporal, Plate* plate, shared_data_mpi_t* shared_data);

/**
 @brief Asigna de forma estática un rango de índices a un proceso MPI

 @param private_data Puntero a la estructura privada donde se almacenarán los
 rangos asignados al proceso
 @param shared_data Puntero a la estructura que contiene la información de
 distribución MPI
 @param plate Estructura que contiene las dimensiones de la lámina que
 determinan el tamaño total del arreglo de temperatura

 Esta función calcula qué rango de índices del arreglo de temperatura debe
 procesar el proceso actual, distribuyendo equitativamente el trabajo. Si el
 número de elementos no es divisible de forma exacta, algunos procesos
 recibirán un elemento adicional.
*/
void dividir_array_mpi(private_data_mpi_t* private_data, shared_data_mpi_t* shared_data, Plate plate);

#endif