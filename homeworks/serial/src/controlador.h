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

char* format_time(const time_t seconds, char* text, const size_t capacity);

void generar_archivo_binario(const char *nombre_archivo, uint64_t R, uint64_t C,
                             double *temperaturas);

void generar_archivo_tsv(const char *directorio, const char *nombreArchivo,
                         Plate plate, double tiempoSegundos, int iteraciones);

#endif
