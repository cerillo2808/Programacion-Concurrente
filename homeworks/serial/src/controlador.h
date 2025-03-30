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

/**
 * @brief Convierte la cantidad dada de @a segundos a un texto aproximado
 * YYYY/MM/DD hh:mm:ss
 * 
 * @param seconds Cantidad de segundos transcurridos desde cualquier punto en
 * el tiempo, no es necesario que sea desde el 1 de enero de 1970 a las
 * 00:00:00 GMT
 * @param text Puntero a un búfer de al menos 48 caracteres
 * @param capacity Capacidad del arreglo apuntado por el búfer, debe ser de 
 * 48 o mayor
 * @return El puntero a text
 */
char* format_time(const time_t seconds, char* text, const size_t capacity);

/**
 @brief Genera un archivo binario con las temperaturas de la placa
 @param nombre_archivo Nombre del archivo binario a generar
 @param R Número de filas de la placa
 @param C Número de columnas de la placa
 @param temperaturas Arreglo con las temperaturas actuales de la placa

 Esta función crea un archivo binario en el directorio "output" con el nombre
  proporcionado en @a nombre_archivo.
 El archivo contiene las dimensiones de la placa (R y C) y los valores de las
  temperaturas en la placa.

 Si el directorio "output" no existe, se crea antes de generar el archivo
  binario.
 Si ocurre algún error al abrir el archivo, la función imprime un mensaje de
  error y termina la ejecución.

 @return No tiene valor de retorno. El archivo binario se genera en el sistema
  de archivos.
*/
void generar_archivo_binario(const char *nombre_archivo, uint64_t R, uint64_t C,
                             double *temperaturas);

/**
 @brief Genera o actualiza un archivo TSV con información de la simulación de la
  placa
 @param directorio Directorio donde se guardará el archivo TSV
 @param nombreArchivo Nombre del archivo TSV a generar o actualizar
 @param plate Estructura que contiene los parámetros de la placa, como
  dimensiones y constantes de simulación
 @param tiempoSegundos Tiempo transcurrido durante la simulación en segundos
 @param iteraciones Número de iteraciones realizadas durante la simulación

 Esta función abre el archivo TSV especificado en el directorio dado y lo
  actualiza (o lo crea si no existe) con los resultados
 de la simulación de la placa. Se guarda el nombre del archivo original, los
  valores de los parámetros de la placa (delta, alfa, h, epsilon),
 el número de iteraciones y el tiempo transcurrido, todo en formato tabulado.

 Si ocurre un error al abrir el archivo, se imprime un mensaje de error.

 @return No tiene valor de retorno. El archivo TSV se actualiza con los datos
  proporcionados.
*/

void generar_archivo_tsv(const char *directorio, const char *nombreArchivo,
                         Plate plate, double tiempoSegundos, int iteraciones);

#endif
