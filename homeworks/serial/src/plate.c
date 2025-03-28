#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <plate.h>
#include <inttypes.h>
#include <controlador.h>

int crear_plate(const char *linea, const char *nombreJob) {

    Plate plate;

    sscanf(linea, "%s %ld %ld %ld %ld", plate.nombreArchivo, &plate.delta, &plate.alfa, &plate.h, &plate.epsilon);
    char rutaCompleta [512];
    // la ruta completa sólo puede medir 512 como máximo
    snprintf(rutaCompleta, sizeof(rutaCompleta), "tests/%s", plate.nombreArchivo);
    // agregarle a nombre de archivo el path completo

    sscanf(nombreJob, "%s", plate.nombreJob);

    // abrir el archivo bin
    FILE *bin = fopen(rutaCompleta, "rb");

    if(leer_plate(rutaCompleta, &plate, bin)){
        return 1;
    }

    // Pedir memoria para matriz que se llama temperaturas
    uint64_t *temperaturas = (uint64_t *)malloc(plate.R * plate.C * sizeof(uint64_t));
    if (!temperaturas) {
        printf("Error: No se pudo asignar memoria para la matriz.\n");
        fclose(bin);
        return 1;
    }

    // Subir temperaturas a la matriz
    fread(temperaturas, sizeof(uint64_t), plate.R * plate.C, bin);

    printf("%s cargado con éxito.\n", plate.nombreArchivo);

    // Simular la dispersión del calor
    cambio_temperatura(temperaturas, plate);

    // Liberar memoria después de la simulación
    free(temperaturas);

    return 0;
}

int leer_plate(const char *nombreBin, Plate *plate, FILE *bin) {
    
    if (!bin) {
        printf("Error: No se pudo abrir el archivo binario '%s'.\n", nombreBin);
        return 1;
    }

    // R por rows, filas en inglés
    uint64_t R;
    // C por columnas
    uint64_t C;

    fread(&R, sizeof(uint64_t), 1, bin);
    fread(&C, sizeof(uint64_t), 1, bin);

    // Verificar que R y C sean válidos
    if (R == 0 || C == 0) {
        printf("Error: Dimensiones inválidas en el archivo binario '%s'.\n", nombreBin);
        fclose(bin);
        return 1;
    }

    plate->R = R;
    plate->C = C;

    return 0;
}   