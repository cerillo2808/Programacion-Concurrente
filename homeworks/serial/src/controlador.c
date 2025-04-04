// Copyright [2025] <Liqing Yosery Zheng Lu>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <plate.h>
#include <inttypes.h>
#include <controlador.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <math.h>

int verificar_argumentos(int argc, char *argv[]) {
    if (argc > 3) {
        // muchos argumentos
        printf("Error: Hay más argumentos de los necesarios. Ingrese la "
                "dirección del archivo y la cantidad de hilos a utilizar.\n");
        return 1;

    } else if (argc <= 1) {
        // faltan argumentos
        printf("Error: Hay menos argumentos de los necesarios. Ingrese la "
                "dirección del archivo y la cantidad de hilos a utilizar.\n");;
        return 1;

    } else {
        // hay al menos un argumento
        // TO-DO: verificar que se haya ingresado cantidad hilos (tarea 2)

        char *jobPath = argv[1];

        FILE *jobFile = fopen(jobPath, "r");
        // el parámetro r es para lectura (read)

        if (jobFile == NULL) {
            printf("Error: No se pudo abrir. Hay un error con el nombre o path "
                    "de su archivo.\n");
            return 1;
        } else {
            // guardar de forma job###
            char nombreJob[256];
            // buscar el / del job
            char *base = strrchr(jobPath, '/');
            if (base) {
                // saltarse el /
                base++;
            } else {
                base = jobPath;
            }

            strncpy(nombreJob, base, sizeof(nombreJob));

            // eliminar la extensión .txt
            char *punto = strrchr(nombreJob, '.');
            if (punto && strcmp(punto, ".txt") == 0) {
                // el punto se vuelve caracter nulo
                *punto = '\0';
            }

            char linea[256];
            // buffer para cada linea de job, máximo de 256 chars

            while (fgets(linea, sizeof(linea), jobFile)) {
                // crear un plate para cada linea del txt
                if (crear_plate(linea, nombreJob)) {
                    return 1;
                }
            }
            fclose(jobFile);
        }
    }
    return 0;
}

void cambio_temperatura(double* temperaturas, Plate plate) {
    int iteraciones = 0;
    double cambio_maximo = 0;
    double cambio = 0;

    // se crea matriz temporal para calcular el cambio
    double *temperaturas_temporal = (double *)malloc(plate.R * plate.C
        * sizeof(double));
    if (!temperaturas_temporal) {
        printf("Error: No se pudo asignar memoria para la matriz temporal.\n");
        return;
    }

    do {
        iteraciones++;
        // se actualiza la variable de iteraciones desde la primera corrida
        cambio_maximo = 0.0;
        // se inicializa la variable en 0
        for (uint64_t i = 0; i < plate.R; i++) {
            for (uint64_t j = 0; j < plate.C; j++) {
                uint64_t indice = i * plate.C + j;
                // se crea un índice único para cada posición de la matriz
                // sirve para ubicar los datos en un arreglo
                if (i == 0 || i == plate.R - 1 || j == 0 || j == plate.C - 1) {
                    // es un borde y se copia como tal
                    temperaturas_temporal[indice] = temperaturas[indice];
                } else {
                    // se consiguen las temperaturas de las placas vecinas
                    double arriba = temperaturas[(i - 1) * plate.C + j];
                    double abajo = temperaturas[(i + 1) * plate.C + j];
                    double izquierda = temperaturas[i * plate.C + (j - 1)];
                    double derecha = temperaturas[i * plate.C + (j + 1)];
                    // se calcula la temperatura con la fórmula de relación
                    temperaturas_temporal[indice] = temperaturas[indice] +
                        plate.alfa * plate.delta *
                        ((arriba + abajo + izquierda + derecha - 4.0 *
                             temperaturas[indice]) / (plate.h * plate.h));
                    // calcular cambio absoluto
                    cambio = fabs(temperaturas_temporal[indice] -
                        temperaturas[indice]);
                    // verificar si el cambio actual fue más que cambio_máximo
                    if (cambio > cambio_maximo) {
                        // en caso de que sí, se actualiza cambio_máximo
                        cambio_maximo = cambio;
                    }
                }
            }
        }
        // actualizar temperaturas, copia matriz temporal a matriz temperatura
        memcpy(temperaturas, temperaturas_temporal, plate.R * plate.C *
             sizeof(double));
    } while (cambio_maximo > plate.epsilon);

    // calcular el tiempo transcurrido
    double tiempoSegundos = iteraciones * plate.delta;

    char nombre_con_iteraciones[512];

    // quitar .bin
    int len = strlen(plate.nombreArchivo);
    if (len > 4 && strcmp(plate.nombreArchivo + len - 4, ".bin") == 0) {
        len -= 4;
    }

    // formato: "nombre-iteraciones.bin"
    snprintf(nombre_con_iteraciones, sizeof(nombre_con_iteraciones),
            "%.*s-%d.bin", len, plate.nombreArchivo, iteraciones);

    generar_archivo_binario(nombre_con_iteraciones, plate.R, plate.C,
         temperaturas);
    char nombre_final_tsv[512];
    snprintf(nombre_final_tsv, sizeof(nombre_final_tsv), "%s%s",
             plate.nombreJob, ".tsv");
    generar_archivo_tsv("output", nombre_final_tsv, plate, tiempoSegundos,
                         iteraciones);
    free(temperaturas_temporal);
}

// Método copiado de la página del profesor Jeisson Hidalgo
// El parámetro de retorno text debe tener al menos 48 caracteres
// (YYYY/MM/DD hh:mm:ss)
char* format_time(const time_t seconds, char* text, const size_t capacity) {
    const struct tm* gmt = gmtime(&seconds); //NOLINT
    snprintf(text, capacity, "%04d/%02d/%02d\t%02d:%02d:%02d", gmt->tm_year
         - 70, gmt->tm_mon, gmt->tm_mday - 1, gmt->tm_hour, gmt->tm_min,
          gmt->tm_sec);
    return text;
}

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

