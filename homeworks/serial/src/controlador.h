#ifndef CONTROLADOR_H
#define CONTROLADOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <plate.h>
#include <inttypes.h>

int verificar_argumentos(int argc, char *argv[]);

void cambio_temperatura(double* temperaturas, Plate plate);

#endif
