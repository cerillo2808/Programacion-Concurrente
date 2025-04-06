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
#include <escritor_archivos.h>
#include <simulacion.h>

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
    snprintf(nombre_final_tsv, sizeof(nombre_final_tsv), "%s%s", //NOLINT
             plate.nombreJob, ".tsv");
    generar_archivo_tsv("output", nombre_final_tsv, plate, tiempoSegundos,
                         iteraciones);
    free(temperaturas_temporal);
}
