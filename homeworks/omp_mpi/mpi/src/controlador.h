// Copyright [2025] <Liqing Yosery Zheng Lu>

#ifndef CONTROLADOR_H
#define CONTROLADOR_H

/**
 *  @brief Controlador de la simulación.
 *  Inicia la simulación y gestiona la comunicación entre el proceso maestro y los trabajadores.
 *  @param argumentos Argumentos de la línea de comandos.
 *  El primer argumento es el nombre del archivo de trabajo, el segundo es el número de iteraciones.
 *  @note Utiliza MPI para la comunicación entre procesos.
 */
void iniciar_simulacion(char *argumentos[]);

/**
 *  @brief Función que ejecuta el trabajo de un trabajador.
 *  Recibe líneas del proceso maestro, procesa cada línea y genera los archivos de salida.
 *  @param argumentos Argumentos de la línea de comandos.
 *  El primer argumento es el nombre del archivo de trabajo, el segundo es el número de iteraciones.
 *  @note Utiliza MPI para la comunicación entre procesos.
 */
void worker_simulacion(char *argumentos[]);

#endif
