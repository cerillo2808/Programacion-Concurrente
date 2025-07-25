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

/**
 @brief Estructura que representa los datos compartidos del programa

 @param nombreJob Nombre del trabajo asociado a la simulación, extraído desde
  la ruta del archivo de entrada

 Esta estructura almacena información común que es utilizada en diferentes
  partes del programa. Principalmente, guarda el nombre base del archivo de
 trabajo, sin ruta ni extensión.
*/
typedef struct shared_data {
  char* nombreJob;
} shared_data_t;

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
int verificar_argumentos(int argc);

#endif
