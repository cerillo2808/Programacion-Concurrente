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

void* procesar_plate_thread(void* arg) {
    private_data_t* private = (private_data_t*)arg;
    private->resultado = crear_plate(private->linea, private->nombreJob);
    return NULL;
}

int verificar_argumentos(int argc, char *argv[]) {
    if (argc > 3) {
        // muchos argumentos
        printf("Error: Hay más argumentos de los necesarios. Ingrese la "
                "dirección del archivo y la cantidad de hilos a utilizar.\n");
        return 1;
    } else if (argc <= 1) {
        // faltan argumentos
        printf("Error: Hay menos argumentos de los necesarios. Ingrese la "
                "dirección del archivo y la cantidad de hilos a utilizar.\n");
        return 1;
    } else {
        // hay al menos un argumento

        // se usa la cantidad de hilos de la computadora por defecto
        uint64_t num_hilos = sysconf(_SC_NPROCESSORS_ONLN);

        // si hay un tercer argumento, es la cantidad de hilos
        if (argc >= 3) {
            num_hilos = atoi(argv[2]);

            // verificar que la cantidad de hilos sea válida
            if (num_hilos <= 0) {
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
            base = base ? base + 1 : jobPath;
            strncpy(nombreJob, base, sizeof(nombreJob));

            // eliminar la extensión .txt
            char *punto = strrchr(nombreJob, '.');
            if (punto && strcmp(punto, ".txt") == 0) {
                // el punto se vuelve caracter nulo
                *punto = '\0';
            }

            // Leer todas las líneas del archivo primero
            // Se asume 100 plates por job
            char lineas[100][256];
            uint64_t num_plates = 0;
            
            while (fgets(lineas[num_plates], sizeof(lineas[0]), jobFile) && num_plates < 100) {
                num_plates++;
            }
            fclose(jobFile);

            // Crear estructuras para mantener el orden
            private_data_t private[num_plates];
            pthread_t hilos[num_plates];
            uint64_t hilos_activos = 0;
            uint64_t error_ocurrido = 0;

            // Procesar plates en bloques para mantener orden de salida
            // Los bloques son la cantidad de plates para cada hilo
            for (uint64_t bloque = 0; bloque < num_plates && !error_ocurrido; bloque += num_hilos) {
                uint64_t hilos_en_bloque = (num_plates - bloque) < num_hilos ? 
                                     (num_plates - bloque) : num_hilos;
                
                // Iniciar hilos para este bloque
                for (uint64_t i = 0; i < hilos_en_bloque; i++) {
                    int plate_actual = bloque + i;
                    private[plate_actual].linea = lineas[plate_actual];
                    private[plate_actual].nombreJob = nombreJob;
                    private[plate_actual].indice = plate_actual;
                    private[plate_actual].resultado = 0;
                    
                    if (pthread_create(&hilos[plate_actual], NULL, 
                                       procesar_plate_thread, &private[plate_actual]) != 0) {
                        printf("Error al crear hilo para plate %d\n", plate_actual);
                        error_ocurrido = 1;
                        break;
                    }
                    hilos_activos++;
                }

                // Esperar a que todos los hilos del bloque terminen
                for (uint64_t i = 0; i < hilos_en_bloque && !error_ocurrido; i++) {
                    int plate_actual = bloque + i;
                    void* retval;
                    pthread_join(hilos[plate_actual], &retval);
                }
            }

            if (error_ocurrido) {
                return 1;
            }

            // Escribir resultados en orden después de que todos los hilos terminen
            for (uint64_t i = 0; i < num_plates; i++) {
                if (private[i].resultado != 0) {
                    printf("Error al procesar plate.");
                    return 1;
                }
            }
        }
    }
    return 0;
}
