// Copyright [2025] <Liqing Yosery Zheng Lu>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <plate.h>
#include <inttypes.h>

Plate crear_plate(const char *linea) {
    Plate plate;

    // carga información a la línea
    sscanf(linea, "%s %lf %lf %lf %lf", plate.nombreArchivo, &plate.delta,
             &plate.alfa, &plate.h, &plate.epsilon);

    return plate;
}

double* leer_plate(const char *nombreJob, Plate *plate, const char *nombreCarpeta) { //NOLINT
    const char *nombreBin = plate->nombreArchivo;

    char rutaCompleta[512];
    // la ruta completa sólo puede medir 512 como máximo

    snprintf(rutaCompleta, sizeof(rutaCompleta), "%s%s", nombreCarpeta,
             nombreBin);

    // agregarle a nombre de archivo el path completo


    sscanf(nombreJob, "%s", plate->nombreJob);

    // abrir el archivo bin
    FILE *bin = fopen(rutaCompleta, "rb");

    if (!bin) {
        printf("Error: No se pudo abrir el archivo binario '%s'.\n", nombreBin);
        return NULL;
    }

    // R por rows, filas en inglés
    uint64_t R;
    // C por columnas
    uint64_t C;

    // leer la cantidad de filas y columnas
    fread(&R, sizeof(uint64_t), 1, bin);
    fread(&C, sizeof(uint64_t), 1, bin);

    // verificar que R y C sean válidos
    if (R == 0 || C == 0) {
        printf("Error: Dimensiones inválidas en el archivo binario '%s'.\n",
                 nombreBin);
        fclose(bin); //NOLINT
        return NULL;
    }

    // asignar valores a los atributos
    plate->R = R;
    plate->C = C;

    // pedir memoria para matriz que se llama temperaturas
    double *temperaturas = (double *)malloc(plate->R *
                                                     plate->C * sizeof(double));
    if (!temperaturas) {
        printf("Error: No se pudo asignar memoria para la matriz.\n");
        fclose(bin);
        return NULL;
    }

    // subir temperaturas a la matriz
    fread(temperaturas, sizeof(double), plate->R * plate->C, bin);
    fclose(bin); //NOLINT

    return temperaturas;
}
