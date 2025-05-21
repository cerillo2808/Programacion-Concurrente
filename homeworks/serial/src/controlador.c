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

int verificar_argumentos(int argc) {
    if (argc > 3) {
        // muchos argumentos
        printf("Error: Hay más argumentos de los necesarios. Ingrese la "
                "dirección del archivo y la cantidad de hilos a utilizar.\n");
        return 0;

    } else if (argc <= 1) {
        // faltan argumentos
        printf("Error: Hay menos argumentos de los necesarios. Ingrese la "
                "dirección del archivo y la cantidad de hilos a utilizar.\n");;
        return 0;

    } else {
        // hay al menos un argumento
        // TO-DO: verificar que se haya ingresado cantidad hilos (tarea 2)

            return 1;
    }
}

int guardarJob(FILE* jobFile, char* jobPath, shared_data_t* shared_data) {
    if (jobFile == NULL) {
        printf("Error: No se pudo abrir. Hay un error con el nombre o path "
                "de su archivo.\n");
        return 0;
    } else {
        // guardar de forma job###
        // el nombreJob puede ser de un máximo de 250 chars
        char* nombreJob = calloc(250, sizeof(char));
        // buscar el / del job
        char *base = strrchr(jobPath, '/');
        if (base) {
            // saltarse el /
            base++;
        } else {
            base = jobPath;
        }

        // se le pasa el tamaño 250 porque es la longitud de nombreJob
        strncpy(nombreJob, base, 250);

        // eliminar la extensión .txt
        char *punto = strrchr(nombreJob, '.');
        if (punto && strcmp(punto, ".txt") == 0) {
            // el punto se vuelve caracter nulo
            *punto = '\0';
        }

        shared_data->nombreJob = nombreJob;
        return 1;
    }
}


int run(int argc, char *argv[]) {
    // inicializa memoria compartida
    shared_data_t* shared_data = (shared_data_t*) calloc(1,
                                                         sizeof(shared_data_t));

    if (verificar_argumentos(argc)) {
        char *jobPath = argv[1];

        FILE *jobFile = fopen(jobPath, "r");
        // el parámetro r es para lectura (read)

        if (guardarJob(jobFile, jobPath, shared_data)) {
            char linea[256];
            // buffer para cada linea de job, máximo de 256 chars

            size_t length = strlen(jobPath);
            // quita el job00x.txt
            ((char*)jobPath)[length - 10] = '\0';

            while (fgets(linea, sizeof(linea), jobFile)) {
                // crear un plate para cada linea del txt
                Plate plate = crear_plate(linea);

                double *temperaturas = leer_plate(shared_data->nombreJob,
                                                        &plate, jobPath);

                if (temperaturas != NULL) {
                    // simular la dispersión del calor
                    // se le pone una particion de 1 porque no se está usando
                    cambio_temperatura(temperaturas, &plate, 1);

                    nombreBin(&plate);

                    generar_archivo_binario(plate.nombreBin, plate.R, plate.C,
                        temperaturas);

                    nombreTsv(&plate);

                    generar_archivo_tsv("output", plate.nombreTsv, plate,
                         plate.tiempoSegundos, plate.iteraciones);
                } else {
                    return 0;
                }

                // liberar memoria después de la simulación
                free(plate.nombreBin);
                free(plate.nombreTsv);
                free(temperaturas);
            }

            fclose(jobFile); //NOLINT
            // liberar memoria compartida
            free(shared_data->nombreJob);
            free(shared_data);
            return 0;
        }
    }

    // hubieron errores en verificarArgumentos
    return 1;
}
