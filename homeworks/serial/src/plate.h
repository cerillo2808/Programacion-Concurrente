#ifndef PLATE_H
#define PLATE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct{
    char *nombreArchivo;

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

#endif