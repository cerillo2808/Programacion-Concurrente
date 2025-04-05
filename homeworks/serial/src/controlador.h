// Copyright [2025] <Liqing Yosery Zheng Lu>

#ifndef CONTROLADOR_H
#define CONTROLADOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <plate.h>
#include <inttypes.h>
#include <time.h>

/**
 @brief Verifica la validez de los argumentos proporcionados al programa y 
 procesa el archivo indicado
 @param argc Número de argumentos pasados al programa
 @param argv Arreglo de cadenas de caracteres que contiene los argumentos
 @return Un código de error:
   0 si los argumentos son válidos y el archivo se procesa correctamente.
   1 si el número de argumentos es incorrecto, si el archivo no se puede abrir o
    si ocurre un error al crear un plate.
 
 El programa espera exactamente 2 argumentos:
   1. La dirección del archivo a procesar
   2. La cantidad de hilos a utilizar (por implementar en la tarea 2)
 
 La función intenta abrir el archivo especificado y, si tiene éxito, extrae el
  nombre del trabajo a partir de la ruta del archivo,
 eliminando la extensión ".txt" si está presente. Luego, lee cada línea del
  archivo y utiliza la función `crear_plate` para crear 
 un plate a partir de la línea leída. Si ocurre algún error durante el proceso,
  se retorna un código de error.
*/
int verificar_argumentos(int argc, char *argv[]);

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

 Al finalizar, genera dos archivos de salida:
   1. Un archivo binario con el nombre del archivo original seguido de la
    cantidad de iteraciones realizadas.
   2. Un archivo .tsv con los resultados de la simulación, incluyendo el tiempo
    transcurrido y el número de iteraciones.

 @return No tiene valor de retorno. Los resultados se guardan en los archivos
  generados.
*/
void cambio_temperatura(double* temperaturas, Plate plate);

#endif
