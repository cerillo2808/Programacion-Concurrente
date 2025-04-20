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

int run(int argc, char *argv[]) {
    // inicializa memoria compartida
    shared_data_t* shared_data = (shared_data_t*) calloc(1,
                                                         sizeof(shared_data_t));

    if (verificar_argumentos(argc, argv, shared_data)) {

        char *jobPath = argv[1];

        FILE *jobFile = fopen(jobPath, "r");
        // el parámetro r es para lectura (read)

        if (guardarJob(jobFile, jobPath, shared_data)) {
            char linea[256];
            // buffer para cada linea de job, máximo de 256 chars

            while (fgets(linea, sizeof(linea), jobFile)) {
                // crear un plate para cada linea del txt
                Plate plate = crear_plate(linea);

                double *temperaturas = leer_plate(shared_data->nombreJob,
                                                                     &plate);

                // inicializa private_data
                private_data_t* private_data = (private_data_t*) calloc(
                    shared_data->cantidadHilos, sizeof(private_data_t));

                // asignarle las temperaturas a la memoria privada
                for (int i = 0; i < shared_data->cantidadHilos; i++) {
                    private_data[i].temperaturas = temperaturas;
                }

                // asignarle la plate a la memoria privada
                for (int i = 0; i < shared_data->cantidadHilos; i++) {
                    private_data[i].plate = &plate;
                }

                if (temperaturas != NULL) {

                    // simular la dispersión del calor
                    int iteraciones = cambio_temperatura(temperaturas, &plate, shared_data);
                    if (iteraciones < 0) {
                        printf("Error: Falló la simulación de transferencia de calor.\n");
                        return 0;
                    }
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
                free(private_data);
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

void dividir_array(private_data_t* private_data, shared_data_t* shared_data, Plate plate) {
    // Calcular el tamaño total del array
    uint64_t total_elementos = plate.R * plate.C;

    // Calcular los bloques de trabajo con mapeo estático
    uint64_t elementos_por_hilo = total_elementos / shared_data->cantidadHilos;
    uint64_t elementos_extra = total_elementos % shared_data->cantidadHilos;
    uint64_t inicio = 0;

    // Asignar los bloques a la memoria privada de cada hilo
    for (int i = 0; i < shared_data->cantidadHilos; i++) {
        private_data[i].inicio = inicio;
        private_data[i].final = inicio + elementos_por_hilo +
                                ((uint64_t)i < elementos_extra ? 1 : 0);
        inicio = private_data[i].final;
    }
}

int verificar_argumentos(int argc, char* argv[], shared_data_t* shared_data) {
    if (argc > 3) {
        // muchos argumentos
        printf("Error: Hay más argumentos de los necesarios. Ingrese la "
               "dirección del archivo y la cantidad de hilos a utilizar.\n");
        return 0;

    } else if (argc <= 1) {
        // faltan argumentos
        printf("Error: Hay menos argumentos de los necesarios. Ingrese la "
               "dirección del archivo y la cantidad de hilos a utilizar.\n");
        return 0;

    } else {
        if (argc == 2) {
            int nucleos = sysconf(_SC_NPROCESSORS_ONLN);
            shared_data->cantidadHilos = nucleos;
            printf("No se ingresó cantidad de hilos, usando los %d núcleos de" 
                " la máquina.\n", shared_data->cantidadHilos);
        } else {
            int cantidad = atoi(argv[2]);
            if (cantidad <= 0) {
                printf("Error: La cantidad de hilos debe ser un número "
                                                        "entero positivo.\n");
                return 0;
            }

            shared_data->cantidadHilos = cantidad;
        }

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
