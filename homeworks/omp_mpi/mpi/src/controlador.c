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
#include <pthread.h>

int run(int argc, char *argv[]) {
    // inicializa memoria compartida
    shared_data_t* shared_data = (shared_data_t*) calloc(1,
                                                         sizeof(shared_data_t));

    if (verificar_argumentos(argc, argv, shared_data)) {
        char *jobPath = argv[1];

        FILE *jobFile = fopen(jobPath, "r");
        // el parámetro r es para lectura (read)

        if (guardarJob(jobFile, jobPath, shared_data)) {
            // Leer todas las líneas del archivo
            char lineas[1024][256];
            int num_lines = 0;
            size_t length = strlen(jobPath);
            // quita el job00x.txt
            ((char*)jobPath)[length - 10] = '\0';
            while (fgets(lineas[num_lines], sizeof(lineas[num_lines]),
                                                                 jobFile)) {
                num_lines++;
            }
            fclose(jobFile);

            // Estructura de datos compartida para reparto dinámico
            dynamic_thread_data_t datos;
            datos.lineas = lineas;
            datos.num_lines = num_lines;
            datos.next_line = 0;
            pthread_mutex_init(&datos.mutex, NULL);
            strncpy(datos.jobPath, jobPath, 256);
            datos.shared_data = shared_data;

            int n_hilos = shared_data->cantidadHilos;
            pthread_t* hilos = malloc(n_hilos * sizeof(pthread_t));
            for (int i = 0; i < n_hilos; i++) {
                pthread_create(&hilos[i], NULL, procesar_plate_thread_dinamico,
                                                                 &datos);
            }

            // Esperar a que todos los hilos terminen
            for (int i = 0; i < n_hilos; i++) {
                pthread_join(hilos[i], NULL);
            }
            pthread_mutex_destroy(&datos.mutex);
            free(hilos);
            free(shared_data->nombreJob);
            free(shared_data);
            return 0;
        }
    }

    // hubieron errores en verificarArgumentos
    return 1;
}

void dividir_array(private_data_t* private_data, shared_data_t* shared_data,
                                                                 Plate plate) {
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
        printf("Error: No se pudo abrir. Hay un error con el" //NOLINT
                                             " nombre o path de su archivo.\n");
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

void* procesar_plate_thread_dinamico(void* arg) {
    dynamic_thread_data_t* data = (dynamic_thread_data_t*)arg;
    while (1) {
        int idx;
        // Sección crítica para obtener el siguiente trabajo
        pthread_mutex_lock(&data->mutex);
        if (data->next_line >= data->num_lines) {
            pthread_mutex_unlock(&data->mutex);
            break;
        }
        idx = data->next_line++;
        pthread_mutex_unlock(&data->mutex);

        // Procesar la línea idx
        Plate plate = crear_plate(data->lineas[idx]);
        double *temperaturas = leer_plate(data->shared_data->nombreJob,
                                                 &plate, data->jobPath);
        if (temperaturas != NULL) {
            cambio_temperatura(temperaturas, &plate, 1);
            nombreBin(&plate);
            generar_archivo_binario(plate.nombreBin, plate.R, plate.C,
                                                                 temperaturas);
            nombreTsv(&plate);
            generar_archivo_tsv("output", plate.nombreTsv, plate,
                                     plate.tiempoSegundos, plate.iteraciones);
            free(plate.nombreBin);
            free(plate.nombreTsv);
            free(temperaturas);
        }
    }
    pthread_exit(NULL);
}
