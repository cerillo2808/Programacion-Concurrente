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
 @brief Realiza la simulación de cambio de temperatura en una placa utilizando
  el método de diferencias finitas
 @param temperaturas Arreglo de temperaturas actuales en la placa
 @param plate Estructura que contiene los parámetros de la placa, como
  dimensiones y constantes de simulación
 Esta función realiza iteraciones de simulación para calcular el cambio de
  temperatura en cada punto de la placa, 
 utilizando un método de diferencias finitas. La simulación se repite hasta que
  el cambio máximo entre las 
 iteraciones es menor que el valor de epsilon especificado en la estructura
  plate.

 @return No tiene valor de retorno. Los resultados se guardan en los archivos
  generados.
*/
int cambio_temperatura(double* temperaturas, Plate* plate,
                                                   shared_data_t* shared_data);
void* cambio_temperatura_hilos(void* arg);

#endif
