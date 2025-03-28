#ifndef PLATE_H
#define PLATE_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

typedef struct{
    char nombreArchivo [256];
    // el nombre del archivo sólo puede tener máximo 256 chars

    char nombreJob [256];

    uint64_t delta;
    // paso de tiempo entre cálculos

    uint64_t alfa;
    // difusividad térmica

    uint64_t h;
    // tamaño de cada celda

    uint64_t epsilon;
    // punto de equilibrio

    uint64_t R;
    // número de filas

    uint64_t C;
    // número de columnas

} Plate;

int crear_plate(const char *linea, const char *nombreJob);

int leer_plate(const char *nombreBin, Plate *plate, FILE *bin);

#endif