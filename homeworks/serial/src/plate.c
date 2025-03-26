#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <plate.h>
#include <inttypes.h>

void crear_plate(const char *linea) {

    Plate plate;

    sscanf(linea, "%s %lf %lf %lf %lf", plate.nombreArchivo, &plate.delta, &plate.alfa, &plate.h, &plate.epsilon);
    char rutaCompleta [512];
    // la ruta completa sólo puede medir 512 como máximo
    snprintf(rutaCompleta, sizeof(rutaCompleta), "tests/%s", plate.nombreArchivo);
    // agregarle a nombre de archivo el path completo

    leer_plate(rutaCompleta);

    printf("%s cargado con éxito.\n", plate.nombreArchivo);
}

void leer_plate(const char *nombreBin) {
    FILE *bin = fopen(nombreBin, "rb");
    if (!bin) {
        printf("Error: No se pudo abrir el archivo binario '%s'\n", nombreBin);
        return;
    }

    uint64_t R, C;
    fread(&R, sizeof(uint64_t), 1, bin);
    fread(&C, sizeof(uint64_t), 1, bin);

    // Verificar que R y C sean válidos
    if (R == 0 || C == 0) {
        printf("Error: Dimensiones inválidas en el archivo binario.\n");
        fclose(bin);
        return;
    }

    // Pedir memoria para matriz que se llama temperaturas
    double *temperaturas = (double *)malloc(R * C * sizeof(double));
    if (!temperaturas) {
        printf("Error: No se pudo asignar memoria para la matriz\n");
        fclose(bin);
        return;
    }

    // Subir temperaturas a la matriz
    fread(temperaturas, sizeof(double), R * C, bin);

    fclose(bin);

    // TO-DO Simulación y escritura de reporte

    // Liberar memoria TEMPORAL
    free(temperaturas);
}   