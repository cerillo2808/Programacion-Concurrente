// Copyright [2025] <Liqing Yosery Zheng Lu>

#ifndef CONTROLADOR_MPI_H
#define CONTROLADOR_MPI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <plate.h>
#include <inttypes.h>
#include <time.h>
#include <simulacion.h>
#include <mpi.h>

/**
 @brief Estructura que representa los datos compartidos del programa con MPI

 @param nombreJob Nombre del trabajo asociado a la simulación, extraído desde
  la ruta del archivo de entrada
 @param rank Identificador único del proceso MPI
 @param size Número total de procesos MPI
 @param inicio Índice inicial del rango de trabajo asignado al proceso
 @param fin Índice final (no inclusivo) del rango de trabajo asignado al proceso
 @param cambio_maximo_global Mayor cambio de temperatura detectado entre
  iteraciones durante la simulación. Se usa como condición de convergencia.

 Esta estructura almacena información común que es utilizada en diferentes
  partes del programa distribuido con MPI. Principalmente, guarda el nombre base 
  del archivo de trabajo y la información de distribución del trabajo.
*/
typedef struct shared_data_mpi {
    char* nombreJob;
    int rank;
    int size;
    int inicio;
    int fin;
    double cambio_maximo_global;
} shared_data_mpi_t;

/**
 @brief Estructura que contiene los datos privados de cada proceso MPI

 @param inicio Índice inicial del rango de celdas que debe procesar el proceso
 @param final Índice final (no inclusivo) del rango de celdas que debe
  procesar el proceso
 @param cambio_maximo_local Máximo cambio de temperatura encontrado por el
  proceso en su rango asignado
 @param temperaturas Puntero a la matriz original de temperaturas
 @param temperaturas_temporal Puntero a la matriz temporal donde el proceso
  almacena los nuevos valores calculados
 @param plate Puntero a la estructura que contiene los parámetros físicos de
  la simulación
 @param shared_data Copia local de los datos compartidos del programa, útil
  para acceso rápido dentro del proceso

 Esta estructura encapsula toda la información necesaria para que cada proceso
 MPI ejecute su porción de la simulación térmica de manera independiente.
*/
typedef struct private_data_mpi {
    uint64_t inicio;
    uint64_t final;
    double cambio_maximo_local;
    double* temperaturas;
    double* temperaturas_temporal;
    Plate* plate;
    shared_data_mpi_t shared_data;
} private_data_mpi_t;

/**
 * @brief Función principal de ejecución del programa con MPI.
 * 
 * Esta función orquesta la simulación de dispersión térmica a partir de un
 * archivo de trabajo (`job`) utilizando MPI para la paralelización.
 * Verifica los argumentos, distribuye el trabajo entre procesos, lee las
 * configuraciones de cada plate, simula la difusión del calor y genera los
 * archivos de salida en formato binario y TSV.
 * 
 * @param argc Número de argumentos de línea de comandos (incluyendo el nombre
 * del programa).
 * @param argv Vector de cadenas con los argumentos. Se espera que `argv[1]`
 * contenga la ruta del archivo job.
 * 
 * @return int Retorna 0 si el programa finaliza exitosamente; 1 si hubo errores
 * en la verificación de argumentos o en el procesamiento.
 */
int run_mpi(int argc, char *argv[]);

/**
 * @brief Distribuye el trabajo entre los procesos MPI de forma estática.
 * 
 * Esta función calcula qué rango de líneas (plates) debe procesar cada proceso
 * MPI, distribuyendo equitativamente el trabajo. Si el número de líneas no es
 * divisible exactamente entre el número de procesos, algunos procesos recibirán
 * una línea adicional.
 * 
 * @param shared_data Puntero a la estructura de datos compartidos MPI
 * @param num_lines Número total de líneas (plates) a procesar
 */
void distribuir_trabajo_mpi(shared_data_mpi_t* shared_data, int num_lines);

/**
 * @brief Guarda el nombre del trabajo desde la ruta del archivo y lo almacena
 * en shared_data para MPI.
 * 
 * Esta función toma el archivo y la ruta del trabajo, extrae el nombre base del
 * archivo (sin la ruta y sin la extensión `.txt`), y lo almacena en la
 * estructura `shared_data_mpi_t`.
 * 
 * @param jobFile Puntero al archivo abierto del trabajo.
 * @param jobPath Ruta del archivo del trabajo como cadena de caracteres.
 * @param shared_data Puntero a una estructura `shared_data_mpi_t` donde se
 * almacenará el nombre del trabajo.
 * 
 * @return int Retorna 1 si el nombre del trabajo fue extraído y guardado
 * exitosamente. Retorna 0 si no se pudo abrir el archivo.
 */
int guardarJob_mpi(FILE* jobFile, char* jobPath, shared_data_mpi_t* shared_data);

/**
 * @brief Verifica los argumentos proporcionados al programa MPI.
 * 
 * Este método comprueba que el número de argumentos proporcionados al ejecutar
 * el programa sea adecuado para MPI. El número de procesos se maneja 
 * externamente con mpirun/mpiexec. Solo se requiere el archivo de entrada.
 * 
 * @param argc Número de argumentos pasados al programa.
 * @param argv Vector de argumentos del programa.
 * @param shared_data Puntero a la estructura de datos compartidos MPI.
 * @return int Retorna 1 si el número de argumentos es correcto, de lo contrario
 * retorna 0.
 */
int verificar_argumentos_mpi(int argc, char* argv[], shared_data_mpi_t* shared_data);

/**
 @brief Asigna de forma estática un rango de índices a cada proceso MPI

 @param private_data Arreglo de estructuras privadas que almacenan los
 rangos asignados a cada proceso
 @param shared_data Puntero a la estructura que contiene la información de
 distribución MPI
 @param plate Estructura que contiene las dimensiones de la lámina que
 determinan el tamaño total del arreglo de temperatura

 Esta función distribuye equitativamente los índices del arreglo de
 temperatura entre los procesos MPI disponibles. Si el número de elementos no es
 divisible de forma exacta, algunos procesos recibirán un elemento adicional.
*/
void dividir_array_mpi(private_data_mpi_t* private_data, shared_data_mpi_t* shared_data,
                       Plate plate);

#endif