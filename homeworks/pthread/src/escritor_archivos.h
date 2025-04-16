// Copyright [2025] <Liqing Yosery Zheng Lu>

#ifndef ESCRITOR_ARCHIVOS_H
#define ESCRITOR_ARCHIVOS_H

#include <plate.h>
#include <time.h>

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

void nombreBin(Plate* plate);

void nombreTsv(Plate* plate);

#endif
