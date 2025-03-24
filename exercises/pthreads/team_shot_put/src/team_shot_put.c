#include <inttypes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief ...
 */
void *tirar_3veces(void *arg);

int main(int argc, char *argv[]) {
  // para verificar que sí haya parámetros
  if (argc < 2) {
    printf("Error: debe pasar la cantidad de atletas como argumento\n");
    // se termina el programa con error
    return 1;
  }

  // convertir el string del input a int
  int cantidadAtletas = atoi(argv[1]);

  // si el input es par, da error
  if (cantidadAtletas % 2 == 0) {
    printf("Error: número par de atletas\n");
    // se termina el programa con error
    return 1;

  } else {
    // no hay errores y se sigue

    // instancio a los atletas
    pthread_t *atletas = malloc(sizeof(pthread_t) * cantidadAtletas * 2);

    double **pizarra = malloc(2 * sizeof(double *));
    for (int i = 0; i < 2; i++) {
      pizarra[i] = malloc(cantidadAtletas * sizeof(double));
    }

    double *resultado;

    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < cantidadAtletas; j++) {
        int *datos = malloc(sizeof(int) * 2);
        // en datos se pasa el equipo y el numero de atleta.
        datos[0] = i;
        datos[1] = j;

        pthread_create(&atletas[2 * i + j], NULL, tirar_3veces, datos);
      }
    }

    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < cantidadAtletas; j++) {
        pthread_join(atletas[2 * i + j], (void **)&resultado);
        // guardar el resultado en la pizarra
        pizarra[i][j] = *resultado;
      }
    }

    // son dos equipos, cada equipo con una cantidad impar de atletas
    int ganes_equipo1 = 0;
    int ganes_equipo2 = 0;

    for (int i = 0; i < cantidadAtletas; i++) {
      if (pizarra[0][i] > pizarra[1][i]) {
        ganes_equipo1++;
      } else if (pizarra[0][i] < pizarra[1][i]) {
        ganes_equipo2++;
      }
      // si es empate nadie tiene punto
    }

    // Imprimir resultados finales
    printf("\nGanes equipo 1: %d\n", ganes_equipo1);
    printf("Ganes equipo 2: %d\n", ganes_equipo2);

    if (ganes_equipo1 < ganes_equipo2) {
      printf("\nGana equipo 2\n");
    } else if (ganes_equipo1 > ganes_equipo2) {
      printf("\nGana equipo 1\n");
    } else {
      printf("\nEmpate\n");
    }

    free(atletas);
    free(pizarra[0]);
    free(pizarra[1]);
    free(pizarra);
  }

  return 0;
}

void *tirar_3veces(void *arg) {
  int *datos = (int *)arg;
  int equipo = datos[0];
  int indice = datos[1];

  unsigned int seed = (unsigned int)time(NULL) ^ (unsigned int)(uintptr_t)pthread_self() ^ indice;
  // la seed depende del índice que se pasó como parámetro

  double *tiro_mas_alto = malloc(sizeof(double));
  double tiro_actual;
  for (int i = 0; i < 3; i++) {
    tiro_actual = ((double)rand_r(&seed) / RAND_MAX) * 20.5;
    if (tiro_actual > *tiro_mas_alto) {
      *tiro_mas_alto = tiro_actual;
    }
  }

  printf("%d.%d: best shot put %f\n", equipo + 1, indice + 1, *tiro_mas_alto);
  
  free(datos);

  return tiro_mas_alto;
}
