#include <inttypes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief ...
 */
void *tirar_3veces(void *arg);
pthread_t* create_threads(size_t count, void*(*routine)(void*), void* data);
int join_threads(size_t count, pthread_t* threads);

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Error: debe pasar la cantidad de atletas como argumento\n");
    return 1;
  }

  int cantidadAtletas = atoi(argv[1]);

  if (cantidadAtletas % 2 == 0) {
    printf("Error: número par de atletas\n");
    return 1;
  }

  pthread_t *atletas = malloc(sizeof(pthread_t) * cantidadAtletas * 2);
  if (!atletas) {
    return 1;
  }

  double **pizarra = malloc(2 * sizeof(double *));
  for (int i = 0; i < 2; i++) {
    pizarra[i] = malloc(cantidadAtletas * sizeof(double));
  }

  double *resultado;

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < cantidadAtletas; j++) {
      int *datos = malloc(sizeof(int) * 2);
      datos[0] = i;
      datos[1] = j;

      pthread_create(&atletas[2 * i + j], NULL, tirar_3veces, datos);
    }
  }

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < cantidadAtletas; j++) {
      pthread_join(atletas[2 * i + j], (void **)&resultado);
      if (resultado) {
        pizarra[i][j] = *resultado;
        
      } else {
        pizarra[i][j] = 0.0;
      }
    }
  }

  int ganes_equipo1 = 0;
  int ganes_equipo2 = 0;

  for (int i = 0; i < cantidadAtletas; i++) {
    if (pizarra[0][i] > pizarra[1][i]) {
      ganes_equipo1++;
    } else if (pizarra[0][i] < pizarra[1][i]) {
      ganes_equipo2++;
    }
  }

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
  return 0;
}

pthread_t* create_threads(size_t count, void*(*routine)(void*), void* data) {
  pthread_t* threads = (pthread_t*) calloc(count, sizeof(pthread_t));
  if (threads) {
    for (size_t index = 0; index < count; ++index) {
      if (pthread_create(&threads[index], NULL, routine, data) != 0) {
        fprintf(stderr, "error: could not create thread %zu\n", index);
        join_threads(index, threads);
        return NULL;
      }
    }
  }
  return threads;
}

int join_threads(size_t count, pthread_t* threads) {
  int error_count = 0;
  for (size_t index = 0; index < count; ++index) {
    const int error = pthread_join(threads[index], NULL);
    if (error) {
      fprintf(stderr, "error: could not join thread %zu\n", index);
      ++error_count;
    }
  }
  free(threads);
  return error_count;
}

void *tirar_3veces(void *arg) {
  int *datos = (int *)arg;
  int equipo = datos[0];
  int indice = datos[1];

  unsigned int seed = (unsigned int)time(NULL) ^ (unsigned int)(uintptr_t)pthread_self() ^ indice;

  double *tiro_mas_alto = malloc(sizeof(double));
  *tiro_mas_alto = 0.0;
  double tiro_actual;
  for (int i = 0; i < 3; i++) {
    tiro_actual = ((double)rand_r(&seed) / RAND_MAX) * 20.5;
    if (tiro_actual > *tiro_mas_alto) {
      *tiro_mas_alto = tiro_actual;
    }
  }

  printf("%d.%d: best shot put %f\n", equipo + 1, indice + 1, *tiro_mas_alto);
  

  return tiro_mas_alto;
}
