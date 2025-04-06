// Copyright [2025] <Liqing Yosery Zheng Lu>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "escritor_archivos.h"

void generar_archivo_binario(const char *nombre_archivo, uint64_t R, uint64_t C,
    double *temperaturas) {
char rutaCompleta[512];

// crear el directorio "output" si no existe
mkdir("output", 0777);

// construir la ruta completa del archivo dentro de "output"
snprintf(rutaCompleta, sizeof(rutaCompleta), "output/%s", nombre_archivo);

FILE *archivo = fopen(rutaCompleta, "wb");
if (archivo == NULL) {
printf("Error al abrir el archivo bin para escritura.\n");
return;
}

fwrite(&R, sizeof(uint64_t), 1, archivo);
fwrite(&C, sizeof(uint64_t), 1, archivo);
fwrite(temperaturas, sizeof(double), R * C, archivo);

fclose(archivo);
}

void generar_archivo_tsv(const char *directorio, const char *nombreArchivo,
Plate plate, double tiempoSegundos, int iteraciones) {
char rutaCompleta[512];
snprintf(rutaCompleta, sizeof(rutaCompleta), "%s/%s", directorio,
nombreArchivo);

FILE *tsvFile = fopen(rutaCompleta, "a");
if (!tsvFile) {
printf("Error: No se pudo abrir el archivo TSV para escritura en %s\n",
rutaCompleta);
return;
}

char tiempo[512];
format_time((time_t)tiempoSegundos, tiempo, sizeof(tiempo));

fprintf(tsvFile, "%s\t", plate.nombreArchivo);

// formatear delta (sin decimales si es entero)
if (plate.delta == (int64_t)plate.delta) {
fprintf(tsvFile, "%ld\t", (int64_t)plate.delta);
} else {
fprintf(tsvFile, "%g\t", plate.delta);
}

// formatear alfa (sin decimales si es entero)
if (plate.alfa == (int64_t)plate.alfa) {
fprintf(tsvFile, "%ld\t", (int64_t)plate.alfa);
} else {
fprintf(tsvFile, "%g\t", plate.alfa);
}

// formatear h (sin decimales si es entero)
if (plate.h == (int64_t)plate.h) {
fprintf(tsvFile, "%ld\t", (int64_t)plate.h);
} else {
fprintf(tsvFile, "%g\t", plate.h);
}

// formatear epsilon (notación científica si es muy pequeño)
if (plate.epsilon < 0.0001) {
fprintf(tsvFile, "%.1e\t", plate.epsilon);
} else {
fprintf(tsvFile, "%g\t", plate.epsilon);
}

// escribir iteraciones y tiempo
fprintf(tsvFile, "%d\t%s\n", iteraciones, tiempo);

fclose(tsvFile); // NOLINT
}

// Método copiado de la página del profesor Jeisson Hidalgo
// El parámetro de retorno text debe tener al menos 48 caracteres
// (YYYY/MM/DD hh:mm:ss)
char* format_time(const time_t seconds, char* text, const size_t capacity) {
    const struct tm* gmt = gmtime(&seconds); //NOLINT
    snprintf(text, capacity, "%04d/%02d/%02d\t%02d:%02d:%02d", //NOLINT
         gmt->tm_year - 70, gmt->tm_mon, gmt->tm_mday - 1, gmt->tm_hour,
         gmt->tm_min, gmt->tm_sec);
    return text;
}
