// Copyright [2025] <Liqing Yosery Zheng Lu>

#ifndef CONTROLADOR_H
#define CONTROLADOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <plate.h>
#include <inttypes.h>
#include <time.h>

int verificar_argumentos(int argc, char *argv[]);

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

void generar_archivo_binario(const char *nombre_archivo, uint64_t R, uint64_t C,
                             double *temperaturas);

void generar_archivo_tsv(const char *directorio, const char *nombreArchivo,
                         Plate plate, double tiempoSegundos, int iteraciones);

#endif
