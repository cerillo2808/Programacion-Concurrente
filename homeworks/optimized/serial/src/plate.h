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
 @param iteraciones Cantidad de transferencias de calor para que la placa
 llegue a equilibrio
 @param nombreBin Nombre del archivo del binario, de forma: plate###.k.bin,
 siendo la k la cantidad de iteraciones
 @param nombreTsv Nombre del archivo del output, de forma: job###.tsv. Se guarda
 en Plate porque cada placa ocupa actualizarlo después de la simulación
 @param tiempoSegundos Lo que duró la simulación de la placa. Se calcula como
 iteraciones*delta. Se mantiene como atributo para facilitar el output

 Esta estructura contiene todos los parámetros necesarios para simular el
  comportamiento térmico de una placa,
 incluyendo la configuración del archivo y los parámetros físicos
  (delta, alfa, h, epsilon) necesarios para los cálculos.
*/
typedef struct {
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

    int iteraciones;
    // cantidad de iteraciones necesarias para que la placa llegue a equilibrio

    char* nombreBin;
    // nombre del archivo del binario, de forma: plate###-k.bin

    char* nombreTsv;
    // nombre del archivo de output, de forma: job###.tsv

    double tiempoSegundos;
    // calculado como iteraciones*delta
} Plate;

/**
 @brief Crea una nueva estructura Plate a partir de una línea de texto

 @param linea Cadena de texto que contiene la información de la placa en el
  formato esperado: nombreArchivo delta alfa h epsilon

 @return Plate Estructura Plate inicializada con los datos extraídos de la
  línea de texto

 Esta función toma una línea de texto, la procesa utilizando `sscanf`, y
  llena los campos de la estructura `Plate` con la información correspondiente.
  Los campos `nombreArchivo`, `delta`, `alfa`, `h`, y `epsilon` se extraen
  de la línea para configurar la placa de acuerdo a esos valores.
*/
Plate crear_plate(const char *linea);

/**
 @brief Lee un archivo binario y carga la matriz de temperaturas en una placa

 @param nombreJob Nombre del trabajo asociado a la simulación
 @param plate Puntero a la estructura Plate que contiene los parámetros de la
  placa a procesar

 @return double* Puntero a un arreglo de temperaturas cargado desde el archivo
  binario, o NULL si ocurre un error

 Esta función abre el archivo binario correspondiente a la placa, lee las
  dimensiones (filas y columnas) del archivo, y luego carga la matriz de
  temperaturas en memoria. Si no se puede abrir el archivo o si hay un error
  en la lectura, se imprime un mensaje de error y se retorna NULL.
*/
double* leer_plate(const char *nombreJob, Plate *plate, const char *nombrePath); // NOLINT

#endif
