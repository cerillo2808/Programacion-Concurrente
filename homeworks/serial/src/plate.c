#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <plate.h>

// Función para leer una línea y crear un objeto Plate
void crear_plate(const char *linea) {
    // Crear un objeto plate
    Plate plate;
    
    // Usar sscanf para leer los datos de la línea
    char filename[256];  // Asumimos un nombre de archivo corto
    sscanf(linea, "%s %lf %lf %lf %lf", filename, &plate.delta, &plate.alfa, &plate.h, &plate.epsilon);
    
    // Asignar el nombre del archivo (copiarlo)
    plate.nombreArchivo = (char *)malloc(strlen(filename) + 1);
    strcpy(plate.nombreArchivo, filename);
    
    // Imprimir para verificar que se leyeron correctamente los datos
    printf("Plate creado: \n");
    printf("Archivo: %s\n", plate.nombreArchivo);
    printf("Duración de cada etapa: %.2f\n", plate.delta);
    printf("Difusividad térmica: %.2f\n", plate.alfa);
    printf("Dimensiones de la celda: %.2f\n", plate.h);
    printf("Sensitividad del punto de equilibrio: %.8f\n", plate.epsilon);
    
    // Liberar la memoria para el nombre del archivo
    free(plate.nombreArchivo);
}