// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
// Más observaciones en el README

#include <assert.h>
#include <inttypes.h>
#include <pthread.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_GREET_LEN 256

//**@var greets es un puntero a un arreglo dinámico con máximo 256 de tamaño */
// thread_shared_data_t
typedef struct shared_data {
  char** greets;
  uint64_t thread_count;
} shared_data_t;

// thread_private_data_t
typedef struct private_data {
  uint64_t thread_number;  // rank
  shared_data_t* shared_data;
} private_data_t;

/**
 * @brief se crean los hilos, se inicializa el arreglo de greets, cada hilo
 * imprime el índice del arreglo greets que corresponde a su ID.
 */
void* greet(void* data);
int create_threads(shared_data_t* shared_data);

// procedure main(argc, argv[])
int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  // create thread_count as result of converting argv[1] to integer
  // thread_count := integer(argv[1])
  uint64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  if (argc == 2) {
    if (sscanf(argv[1], "%" SCNu64, &thread_count) == 1) {
    } else {
      fprintf(stderr, "Error: invalid thread count\n");
      return 11;
    }
  }

  shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));
  if (shared_data) {
    shared_data->greets = (char**) calloc(thread_count, sizeof(char*));
    shared_data->thread_count = thread_count;

    if (shared_data->greets) {

      struct timespec start_time, finish_time;
      clock_gettime(CLOCK_MONOTONIC, &start_time);

      error = create_threads(shared_data);

      clock_gettime(CLOCK_MONOTONIC, &finish_time);
      double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
        (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;

      printf("Execution time: %.9lfs\n", elapsed_time);

      free(shared_data->greets);
    } else {
      fprintf(stderr, "Error: could not allocate greets\n");
      error = 13;
    }
    free(shared_data);
  } else {
    fprintf(stderr, "Error: could not allocate shared data\n");
    error = 12;
  }
  return error;
}  // end procedure


int create_threads(shared_data_t* shared_data) {
  int error = EXIT_SUCCESS;
  // for thread_number := 0 to thread_count do
  pthread_t* threads = (pthread_t*)
    malloc(shared_data->thread_count * sizeof(pthread_t));
  private_data_t* private_data = (private_data_t*)
    calloc(shared_data->thread_count, sizeof(private_data_t));
  if (threads && private_data) {
    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
      shared_data->greets[thread_number] = (char*)
        malloc(MAX_GREET_LEN * sizeof(char));
      if (shared_data->greets[thread_number]) {
        // *shared_data->greets[thread_number] = '\0';
        shared_data->greets[thread_number][0] = '\0';
        private_data[thread_number].thread_number = thread_number;
        private_data[thread_number].shared_data = shared_data;
        // create_thread(greet, thread_number)
        error = pthread_create(&threads[thread_number], /*attr*/ NULL, greet
          , /*arg*/ &private_data[thread_number]);
        if (error == EXIT_SUCCESS) {
        } else {
          fprintf(stderr, "Error: could not create secondary thread\n");
          error = 21;
          break;
        }

      } else {
        fprintf(stderr, "Error: could not init semaphore\n");
        error = 22;
        break;
      }
    }

    // print "Hello from main thread"
    printf("Hello from main thread\n");

    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
      pthread_join(threads[thread_number], /*value_ptr*/ NULL);

    }

    // for thread_number := 0 to thread_count do
    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
      // print greets[thread_number]
      printf("%s\n", shared_data->greets[thread_number]);
      free(shared_data->greets[thread_number]);
    }  // end for

    free(private_data);
    free(threads);
  } else {
    fprintf(stderr, "Error: could not allocate %" PRIu64 " threads\n"
      , shared_data->thread_count);
    error = 22;
  }

  return error;
}

// procedure greet:
void* greet(void* data) {
  assert(data);
  private_data_t* private_data = (private_data_t*) data;
  shared_data_t* shared_data = private_data->shared_data;

  // greets[thread_number] := format("Hello from secondary thread"
  // , thread_number, " of ", thread_count)
  sprintf(shared_data->greets[private_data->thread_number]
    , "Hello from secondary thread %" PRIu64 " of %" PRIu64
    , private_data->thread_number, shared_data->thread_count);
  return NULL;
}  // end procedure