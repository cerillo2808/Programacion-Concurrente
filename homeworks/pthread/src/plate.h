// Copyright [2025] <Liqing Yosery Zheng Lu>

#ifndef PLATE_H
#define PLATE_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

/**
 @brief Estructura que representa los parámetros y características de una placa
  en la simulación térmica
 @param nombreArchivo Nombre del archivo asociado a la simulación
 @param nombreJob Nombre del trabajo asociado a la simulación
 @param delta Paso de tiempo entre cálculos de temperatura
 @param alfa Difusividad térmica de la placa
 @param h Tamaño de cada celda en la placa
 @param epsilon Punto de equilibrio para la simulación (tolerancia para detener
  la iteración)
 @param R Número de filas de la placa
 @param C Número de columnas de la placa

 Esta estructura contiene todos los parámetros necesarios para simular el
  comportamiento térmico de una placa,
 incluyendo la configuración del archivo y los parámetros físicos
  (delta, alfa, h, epsilon) necesarios para los cálculos.
*/
typedef struct Plate {
    char nombreArchivo[256];
    // el nombre del archivo sólo puede tener máximo 256 chars

    char nombreJob[256];

    double delta;
    // paso de tiempo entre cálculos

    double alfa;
    // difusividad térmica

    double h;
    // tamaño de cada celda

    double epsilon;
    // punto de equilibrio

    uint64_t R;
    // número de filas

    uint64_t C;
    // número de columnas
} Plate;

/**
 @brief Crea una plate utilizando la información proporcionada en @a linea y
  @a nombreJob
 @param linea Cadena de caracteres que contiene la información para crear la
  plate
 @param nombreJob Cadena de caracteres con el nombre del trabajo asociado
 @return Un código de error:
   0 si la operación fue exitosa.
   1 si ocurre un error al crear la plate
*/
int crear_plate(const char *linea, const char *nombreJob);

/**
 @brief Lee los datos de una placa desde un archivo binario y los almacena en la
  estructura @a plate
 @param nombreBin Cadena de caracteres que contiene el nombre del archivo
  binario
 @param plate Puntero a la estructura Plate donde se almacenarán los datos
  leídos
 @param bin Puntero al archivo binario abierto para lectura
 @return Un código de error:
   0 si la operación fue exitosa.
   1 si ocurre un error al abrir o leer el archivo
*/
int leer_plate(const char *nombreBin, Plate *plate, FILE *bin); // NOLINT

#endif
