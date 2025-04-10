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
#include <unistd.h>

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

        // se usa la cantidad de hilos de la computadora por defecto
        uint64_t num_hilos = sysconf(_SC_NPROCESSORS_ONLN);

        // si hay un tercer argumento, es la cantidad de hilos
        if (argc >= 3) {
            num_hilos = atoi(argv[2]);

            // verificar que la cantidad de hilos sea válida
            if (num_hilos >= 0) {
                printf("Error: tiene que haber mínimo un hilo.");
                return 1;
            }
        }

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
            fclose(jobFile); //NOLINT
        }
    }
    return 0;
}
