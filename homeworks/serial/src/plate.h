#ifndef PLATE_H
#define PLATE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct{
    char nombreArchivo [256];
    // el nombre del archivo sólo puede tener máximo 256 chars

    double delta;
    // paso de tiempo entre cálculos

    double alfa;
    // difusividad térmica

    double h;
    // tamaño de cada celda

    double epsilon;
    // punto de equilibrio

} Plate;

void crear_plate(const char *linea);

void leer_plate(const char *nombreBin);


#endif