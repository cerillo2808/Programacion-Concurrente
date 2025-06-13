// Copyright [2025] <Liqing Yosery Zheng Lu>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <plate.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <math.h>
#include <simulacion.h>
#include <mpi.h>
#include <controlador.h>

void cambio_temperatura_mpi(double* temperaturas, Plate* plate, shared_data_mpi_t* shared_data) {
    int iteraciones = 0;
    double cambio_maximo_local = 0.0;
    double cambio_maximo_global = 0.0;
    
    // Crear matriz temporal para calcular el cambio
    double *temperaturas_temporal = (double *)malloc(plate->R * plate->C * sizeof(double));
    if (!temperaturas_temporal) {
        printf("Proceso %d: Error: No se pudo asignar memoria para la matriz temporal.\n", shared_data->rank);
        return;
    }
    
    // Distribuir trabajo entre procesos
    private_data_mpi_t private_data;
    dividir_array_mpi(&private_data, shared_data, *plate);
    private_data.temperaturas = temperaturas;
    private_data.temperaturas_temporal = temperaturas_temporal;
    private_data.plate = plate;
    
    do {
        iteraciones++;
        cambio_maximo_local = 0.0;
        
        // Cada proceso calcula su parte
        calcular_iteracion_mpi(&private_data);
        
        // Reducción para encontrar el cambio máximo global
        MPI_Allreduce(&private_data.cambio_maximo_local, &cambio_maximo_global, 
                      1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
        
        // Reunir todos los datos en el proceso 0
        reunir_temperaturas_mpi(temperaturas_temporal, plate, shared_data);
        
        // Broadcast de las nuevas temperaturas a todos los procesos
        MPI_Bcast(temperaturas_temporal, plate->R * plate->C, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        
        // Copiar temperaturas temporales a las originales
        memcpy(temperaturas, temperaturas_temporal, plate->R * plate->C * sizeof(double));
        
    } while (cambio_maximo_global > plate->epsilon);
    
    plate->iteraciones = iteraciones;
    plate->tiempoSegundos = iteraciones * plate->delta;
    
    free(temperaturas_temporal);
}

void calcular_iteracion_mpi(private_data_mpi_t* private_data) {
    private_data->cambio_maximo_local = 0.0;
    
    for (uint64_t idx = private_data->inicio; idx < private_data->final; idx++) {
        uint64_t i = idx / private_data->plate->C;
        uint64_t j = idx % private_data->plate->C;
        
        // Condiciones de borde: mantener temperatura original
        if (i == 0 || i == private_data->plate->R - 1 || 
            j == 0 || j == private_data->plate->C - 1) {
            private_data->temperaturas_temporal[idx] = private_data->temperaturas[idx];
        } else {
            // Cálculo de diferencias finitas
            double arriba = private_data->temperaturas[(i - 1) * private_data->plate->C + j];
            double abajo = private_data->temperaturas[(i + 1) * private_data->plate->C + j];
            double izquierda = private_data->temperaturas[i * private_data->plate->C + (j - 1)];
            double derecha = private_data->temperaturas[i * private_data->plate->C + (j + 1)];
            
            private_data->temperaturas_temporal[idx] = private_data->temperaturas[idx] +
                private_data->plate->alfa * private_data->plate->delta *
                ((arriba + abajo + izquierda + derecha - 4.0 * private_data->temperaturas[idx]) / 
                 (private_data->plate->h * private_data->plate->h));
            
            // Calcular cambio y actualizar máximo local
            double cambio = fabs(private_data->temperaturas_temporal[idx] - private_data->temperaturas[idx]);
            if (cambio > private_data->cambio_maximo_local) {
                private_data->cambio_maximo_local = cambio;
            }
        }
    }
}

void reunir_temperaturas_mpi(double* temperaturas_temporal, Plate* plate, shared_data_mpi_t* shared_data) {
    // Calcular cuántos elementos tiene cada proceso
    uint64_t total_elementos = plate->R * plate->C;
    int* recvcounts = NULL;
    int* displs = NULL;
    
    if (shared_data->rank == 0) {
        recvcounts = (int*)malloc(shared_data->size * sizeof(int));
        displs = (int*)malloc(shared_data->size * sizeof(int));
        
        uint64_t elementos_por_proceso = total_elementos / shared_data->size;
        uint64_t elementos_extra = total_elementos % shared_data->size;
        
        for (int i = 0; i < shared_data->size; i++) {
            recvcounts[i] = elementos_por_proceso + (i < elementos_extra ? 1 : 0);
            displs[i] = (i == 0) ? 0 : displs[i-1] + recvcounts[i-1];
        }
    }
    
    // Calcular cuántos elementos tiene este proceso
    uint64_t elementos_por_proceso = total_elementos / shared_data->size;
    uint64_t elementos_extra = total_elementos % shared_data->size;
    int mi_count = elementos_por_proceso + (shared_data->rank < elementos_extra ? 1 : 0);
    
    // Reunir todos los datos en el proceso 0
    MPI_Gatherv(&temperaturas_temporal[shared_data->inicio], mi_count, MPI_DOUBLE,
                temperaturas_temporal, recvcounts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    if (shared_data->rank == 0) {
        free(recvcounts);
        free(displs);
    }
}

void dividir_array_mpi(private_data_mpi_t* private_data, shared_data_mpi_t* shared_data, Plate plate) {
    // Calcular el tamaño total del array
    uint64_t total_elementos = plate.R * plate.C;
    
    // Calcular los bloques de trabajo con mapeo estático
    uint64_t elementos_por_proceso = total_elementos / shared_data->size;
    uint64_t elementos_extra = total_elementos % shared_data->size;
    
    // Calcular inicio y final para este proceso
    private_data->inicio = shared_data->rank * elementos_por_proceso + 
                          (shared_data->rank < elementos_extra ? shared_data->rank : elementos_extra);
    private_data->final = private_data->inicio + elementos_por_proceso + 
                         (shared_data->rank < elementos_extra ? 1 : 0);
    
    // Inicializar cambio máximo local
    private_data->cambio_maximo_local = 0.0;
}