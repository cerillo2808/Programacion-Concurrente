// Copyright [2025] <Liqing Yosery Zheng Lu>

#ifndef CONTROLADOR_H
#define CONTROLADOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <plate.h>
#include <inttypes.h>
#include <time.h>
#include <simulacion.h>
#include <pthread.h>

/**
 @brief Estructura que representa los datos compartidos del programa

 @param nombreJob Nombre del trabajo asociado a la simulación, extraído desde
  la ruta del archivo de entrada
  @param cantidadHilos Cantidad de hilos que el usuario ingresa o la cantidad
  de núcleos que tiene la computadora
  @param cambio_maximo_global Mayor cambio de temperatura detectado entre
 iteraciones durante la simulación. Se usa como condición de convergencia.

 Esta estructura almacena información común que es utilizada en diferentes
  partes del programa. Principalmente, guarda el nombre base del archivo de
 trabajo, sin ruta ni extensión.
*/
typedef struct shared_data {
  char* nombreJob;
  int cantidadHilos;
  double cambio_maximo_global;
} shared_data_t;

/**
 @brief Estructura que contiene los datos privados de cada hilo

 @param inicio Índice inicial del rango de celdas que debe procesar el hilo
 @param final Índice final (no inclusivo) del rango de celdas que debe
 procesar el hilo
 @param cambio_maximo_local Máximo cambio de temperatura encontrado por el
 hilo en su rango asignado
 @param temperaturas Puntero a la matriz original de temperaturas
 @param temperaturas_temporal Puntero a la matriz temporal donde el hilo
 almacena los nuevos valores calculados
 @param plate Puntero a la estructura que contiene los parámetros físicos de
 la simulación
 @param shared_data Copia local de los datos compartidos del programa, útil
 para acceso rápido dentro del hilo

 Esta estructura encapsula toda la información necesaria para que cada hilo
 ejecute su porción de la simulación térmica de manera independiente.
*/
typedef struct private_data {
  uint64_t inicio;
  uint64_t final;
  double cambio_maximo_local;
  double* temperaturas;
  double* temperaturas_temporal;
  Plate* plate;
  shared_data_t shared_data;
} private_data_t;

/**
 * @brief Función principal de ejecución del programa.
 * 
 * Esta función orquesta la simulación de dispersión térmica a partir de un
 * archivo de trabajo (`job`).
 * Verifica los argumentos, abre el archivo indicado, lee línea por línea la
 * configuración de cada plate, simula la difusión del calor y genera los
 * archivos de salida en formato binario y TSV.
 * 
 * @param argc Número de argumentos de línea de comandos (incluyendo el nombre
 * del programa).
 * @param argv Vector de cadenas con los argumentos. Se espera que `argv[1]`
 * contenga la ruta del archivo job.
 * 
 * @return int Retorna 0 si el programa finaliza exitosamente; 1 si hubo errores
 * en la verificación de argumentos o en el procesamiento.
 */
int run(int argc, char *argv[]);

/**
 * @brief Guarda el nombre del trabajo desde la ruta del archivo y lo almacena
 * en shared_data.
 * 
 * Esta función toma el archivo y la ruta del trabajo, extrae el nombre base del
 * archivo (sin la ruta y sin la extensión `.txt`), y lo almacena en la
 * estructura `shared_data`.
 * 
 * @param jobFile Puntero al archivo abierto del trabajo.
 * @param jobPath Ruta del archivo del trabajo como cadena de caracteres.
 * @param shared_data Puntero a una estructura `shared_data_t` donde se
 * almacenará el nombre del trabajo.
 * 
 * @return int Retorna 1 si el nombre del trabajo fue extraído y guardado
 * exitosamente.
 * Retorna 0 si no se pudo abrir el archivo (es decir, si `jobFile` es NULL).
 */
int guardarJob(FILE* jobFile, char* jobPath, shared_data_t* shared_data); //NOLINT

/**
 * @brief Verifica los argumentos proporcionados al programa.
 * 
 * Este método comprueba que el número de argumentos proporcionados al ejecutar
 * el programa sea adecuado. Si el número de argumentos es incorrecto, se
 * muestra un mensaje de error. El número esperado de argumentos es entre 2 y 3,
 * donde el primer argumento es la dirección del archivo y el segundo (opcional)
 * es la cantidad de hilos a utilizar.
 * 
 * @param argc Número de argumentos pasados al programa.
 * @return int Retorna 1 si el número de argumentos es correcto, de lo contrario
 * retorna 0.
 */
int verificar_argumentos(int argc, char* argv[], shared_data_t* shared_data);

/**
 @brief Asigna de forma estática un rango de índices a cada hilo

 @param private_data Arreglo de estructuras privadas que almacenan los
 rangos asignados a cada hilo
 @param shared_data Puntero a la estructura que contiene la cantidad de hilos
 disponibles para la simulación
 @param plate Estructura que contiene las dimensiones de la lámina que
 determinan el tamaño total del arreglo de temperatura

 Esta función distribuye equitativamente los índices del arreglo de
 temperatura entre los hilos disponibles. Si el número de elementos no es
 divisible de forma exacta, algunos hilos recibirán un elemento adicional.
*/
void dividir_array(private_data_t* private_data, shared_data_t* shared_data,
                                                                 Plate plate);

#endif
