// Copyright [2025] <Liqing Yosery Zheng Lu>

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/**
 * @brief ...
 */
pthread_t* create_threads(size_t count, void*(*routine)(void*), void* data);
int join_threads(size_t count, pthread_t* threads);
void *pegar_pinata(void *arg);

typedef struct shared_data {
    int cantidad_hilos;
    int golpes_maximos;
    bool esta_rota;
    pthread_mutex_t puede_golpear;
} shared_data_t;

typedef struct private_data {
    int ID_hilo;
    shared_data_t *shared_data;
} private_data_t;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Ingrese la cantidad de hilos y la dureza de la piñata.\n");
        return 1;
    }

    int cantidad_hilos = atoi(argv[1]);
    int golpes_maximos = atoi(argv[2]);

    shared_data_t *shared_data = (shared_data_t *)malloc(sizeof(shared_data_t));
    shared_data->golpes_maximos = golpes_maximos;
    shared_data->esta_rota = false;
    shared_data->cantidad_hilos = cantidad_hilos;

    pthread_mutex_init(&shared_data->puede_golpear, NULL);

    struct timespec start_time, finish_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    pthread_t* threads = create_threads(cantidad_hilos, pegar_pinata, (void *)shared_data);
    join_threads(cantidad_hilos, threads);

    clock_gettime(CLOCK_MONOTONIC, &finish_time);
    double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
      (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;

    printf("Execution time: %.9lfs\n", elapsed_time);

    pthread_mutex_destroy(&shared_data->puede_golpear);

    free(shared_data);
    return 0;
}

void *pegar_pinata(void *arg) {
    private_data_t *private_data = (private_data_t *)arg;
    shared_data_t *shared_data = private_data->shared_data;

    pthread_mutex_lock(&shared_data->puede_golpear);

    // se calculan los golpes que cada hilo puede dar
    int base_golpes = shared_data->golpes_maximos / shared_data->cantidad_hilos;
    int golpes_restantes = shared_data->golpes_maximos % shared_data->cantidad_hilos;

    // random para ver cuántas veces le pega cada hilo
    unsigned int seed = (unsigned int)time(NULL) + private_data->ID_hilo;
    int cantidad_golpes = base_golpes;

    if (golpes_restantes > 0) {
        int extra = rand_r(&seed) % (golpes_restantes + 1);
        cantidad_golpes += extra;
        golpes_restantes -= extra;
    }

    // si es el último hilo, dejar que rompa la piñata
    if (shared_data->cantidad_hilos - 1 == private_data->ID_hilo) {
        cantidad_golpes = shared_data->golpes_maximos;
        shared_data->golpes_maximos -= cantidad_golpes;
      } else {
        // si no, simplemente se le resta la cantidad de golpes a la piñata
        shared_data->golpes_maximos -= cantidad_golpes;
      }

      // se imprime el hilo que rompió la piñata
      if (shared_data->golpes_maximos == 0 && shared_data->esta_rota == false) {
        // si la piñata ya no puede soportar más golpes, es porque está rota
        printf("Hilo %d/%d %d hits, rompió la piñata.\n", private_data->ID_hilo, shared_data->cantidad_hilos, cantidad_golpes);
        shared_data->esta_rota = true;

      } else {
        // se imprime el hilo y la cantidad de golpes
        printf("Hilo %d/%d %d hits.\n", private_data->ID_hilo, shared_data->cantidad_hilos, cantidad_golpes);
      }
      // se desbloquea el mutex
      pthread_mutex_unlock(&shared_data->puede_golpear);

      free(private_data);

    return NULL;
}

pthread_t* create_threads(size_t count, void*(*routine)(void*), void* data) {
    pthread_t* threads = (pthread_t*) calloc(count, sizeof(pthread_t));
    if (threads) {
        for (size_t index = 0; index < count; ++index) {
            private_data_t *private_data = malloc(sizeof(private_data_t));
            private_data->ID_hilo = index;
            private_data->shared_data = (shared_data_t*)data;

            if (pthread_create(&threads[index], NULL, routine, private_data) != 0) {
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
