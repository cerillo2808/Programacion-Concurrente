#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <plate.h>

void crear_plate(const char *linea) {

    Plate plate;

    sscanf(linea, "%s %lf %lf %lf %lf", plate.nombreArchivo, &plate.delta, &plate.alfa, &plate.h, &plate.epsilon);
    
    
}