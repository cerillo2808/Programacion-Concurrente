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

typedef struct shared_data {
  char* nombreJob;
} shared_data_t;

int run(int argc, char *argv[]);

int guardarJob(FILE* jobFile, char* jobPath, shared_data_t* shared_data);

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
int verificar_argumentos(int argc);

#endif
