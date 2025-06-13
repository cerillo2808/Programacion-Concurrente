// Copyright [2025] <Liqing Yosery Zheng Lu>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <plate.h>
#include <inttypes.h>
#include <controlador.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <math.h>
#include <escritor_archivos.h>
#include <simulacion.h>
#include <unistd.h>

int run_mpi(int argc, char *argv[]) {
    int rank, size;
    
    // Inicializar MPI
    // MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    // Inicializar memoria compartida
    shared_data_mpi_t* shared_data = (shared_data_mpi_t*) calloc(1, sizeof(shared_data_mpi_t));
    shared_data->rank = rank;
    shared_data->size = size;
    
    int result = 0;
    char **lineas = NULL;
    int num_lines = 0;
    char jobPath[512];
    
    // Solo el proceso 0 verifica argumentos y lee el archivo
    if (rank == 0) {
        if (!verificar_argumentos_mpi(argc, argv, shared_data)) {
            result = 1;
        } else {
            strcpy(jobPath, argv[1]);
            FILE *jobFile = fopen(jobPath, "r");
            
            if (!guardarJob_mpi(jobFile, jobPath, shared_data)) {
                result = 1;
            } else {
                // Leer todas las líneas del archivo
                char temp_lineas[1024][256];
                size_t length = strlen(jobPath);
                // Quitar el job00x.txt
                jobPath[length - 10] = '\0';
                
                while (fgets(temp_lineas[num_lines], sizeof(temp_lineas[num_lines]), jobFile)) {
                    // Remover salto de línea
                    temp_lineas[num_lines][strcspn(temp_lineas[num_lines], "\n")] = 0;
                    num_lines++;
                }
                fclose(jobFile);
                
                // Allocar memoria para las líneas
                lineas = (char**)malloc(num_lines * sizeof(char*));
                for (int i = 0; i < num_lines; i++) {
                    lineas[i] = (char*)malloc(256 * sizeof(char));
                    strcpy(lineas[i], temp_lineas[i]);
                }
            }
        }
    }
    
    // Broadcast del resultado de verificación
    MPI_Bcast(&result, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (result != 0) {
        if (shared_data->nombreJob) free(shared_data->nombreJob);
        free(shared_data);
        MPI_Finalize();
        return 1;
    }
    
    // Broadcast de datos necesarios
    MPI_Bcast(&num_lines, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(jobPath, 512, MPI_CHAR, 0, MPI_COMM_WORLD);
    
    // Broadcast del nombre del job
    int nombreJob_len = 0;
    if (rank == 0) {
        nombreJob_len = strlen(shared_data->nombreJob) + 1;
    }
    MPI_Bcast(&nombreJob_len, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    if (rank != 0) {
        shared_data->nombreJob = (char*)malloc(nombreJob_len);
    }
    MPI_Bcast(shared_data->nombreJob, nombreJob_len, MPI_CHAR, 0, MPI_COMM_WORLD);
    
    // Los procesos distintos de 0 necesitan allocar memoria para las líneas
    if (rank != 0) {
        lineas = (char**)malloc(num_lines * sizeof(char*));
        for (int i = 0; i < num_lines; i++) {
            lineas[i] = (char*)malloc(256 * sizeof(char));
        }
    }
    
    // Broadcast de todas las líneas
    for (int i = 0; i < num_lines; i++) {
        MPI_Bcast(lineas[i], 256, MPI_CHAR, 0, MPI_COMM_WORLD);
    }
    
    // Distribución de trabajo usando mapeo estático
    distribuir_trabajo_mpi(shared_data, num_lines);
    
    // Procesar las plates asignadas a este proceso
    for (int i = shared_data->inicio; i < shared_data->fin; i++) {
        Plate plate = crear_plate(lineas[i]);
        double *temperaturas = leer_plate(shared_data->nombreJob, &plate, jobPath);
        
        if (temperaturas != NULL) {
            // Realizar simulación con MPI
            cambio_temperatura_mpi(temperaturas, &plate, shared_data);
            
            // Solo el proceso 0 genera archivos de salida
            if (rank == 0) {
                nombreBin(&plate);
                generar_archivo_binario(plate.nombreBin, plate.R, plate.C, temperaturas);
                nombreTsv(&plate);
                generar_archivo_tsv("output", plate.nombreTsv, plate, plate.tiempoSegundos, plate.iteraciones);
                free(plate.nombreBin);
                free(plate.nombreTsv);
            }
            
            free(temperaturas);
        }
    }
    
    // Limpiar memoria
    if (lineas) {
        for (int i = 0; i < num_lines; i++) {
            free(lineas[i]);
        }
        free(lineas);
    }
    
    free(shared_data->nombreJob);
    free(shared_data);
    
    MPI_Finalize();
    return 0;
}

void distribuir_trabajo_mpi(shared_data_mpi_t* shared_data, int num_lines) {
    int elementos_por_proceso = num_lines / shared_data->size;
    int elementos_extra = num_lines % shared_data->size;
    
    shared_data->inicio = shared_data->rank * elementos_por_proceso + 
                         (shared_data->rank < elementos_extra ? shared_data->rank : elementos_extra);
    shared_data->fin = shared_data->inicio + elementos_por_proceso + 
                      (shared_data->rank < elementos_extra ? 1 : 0);
}

int verificar_argumentos_mpi(int argc, char* argv[], shared_data_mpi_t* shared_data) {
    if (argc > 2) {
        // MPI maneja la cantidad de procesos externamente
        printf("Error: Demasiados argumentos. Solo ingrese la dirección del archivo.\n");
        return 0;
    } else if (argc <= 1) {
        printf("Error: Falta el argumento del archivo. Ingrese la dirección del archivo.\n");
        return 0;
    }
    
    return 1;
}

int guardarJob_mpi(FILE* jobFile, char* jobPath, shared_data_mpi_t* shared_data) {
    if (jobFile == NULL) {
        printf("Error: No se pudo abrir. Hay un error con el nombre o path de su archivo.\n");
        return 0;
    } else {
        // Guardar de forma job###
        char* nombreJob = calloc(250, sizeof(char));
        
        // Buscar el / del job
        char *base = strrchr(jobPath, '/');
        if (base) {
            base++; // Saltarse el /
        } else {
            base = jobPath;
        }
        
        strncpy(nombreJob, base, 250);
        
        // Eliminar la extensión .txt
        char *punto = strrchr(nombreJob, '.');
        if (punto && strcmp(punto, ".txt") == 0) {
            *punto = '\0';
        }
        
        shared_data->nombreJob = nombreJob;
        return 1;
    }
}