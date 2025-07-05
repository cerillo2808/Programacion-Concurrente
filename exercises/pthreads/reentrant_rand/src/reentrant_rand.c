// Copyright 2021-2024 ECCI-UCR CC-BY 4.0
#define _DEFAULT_SOURCE

#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// my_random library ----------------------------------------------------------

// GCC values from https://en.wikipedia.org/wiki/Linear_congruential_generator
#define MODULUS 2147483648l
#define MULTIPLIER 1103515245l
#define INCREMENT 12345l

typedef struct {
  int actual;
} my_rand_state_t;

int my_actual = 0;

void my_seed(int seed) {
  my_actual = seed;
}

int my_rand(int min, int max) {
  const long next = (MULTIPLIER * my_actual + INCREMENT) % MODULUS;
  my_actual = next;
  return min + next % (max - min);
}

void my_seed_r(my_rand_state_t* state, int seed) {
  state->actual = seed;
}

int my_rand_r(my_rand_state_t* state, int min, int max) {
  const long next = (MULTIPLIER * state->actual + INCREMENT) % MODULUS;
  state->actual = next;
  return min + next % (max - min);
}

// tester program -------------------------------------------------------------

int count = 0;
int* numbers = NULL;
int min = 0;
int max = 0;
size_t thread_count = 0;

void* generate_r(void* data);

int main(int argc, char* argv[]) {
  count = argc >= 2 ? atoi(argv[1]) : 1;
  min = argc >= 3 ? atoi(argv[2]) : 0;
  max = argc >= 4 ? atoi(argv[3]) : 100;
  const int seed = argc >= 5 ? atoi(argv[4]) : clock() % MODULUS;

  my_seed(seed);
  numbers = (int*) calloc(count, sizeof(int));
  assert(numbers);

  thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  pthread_t threads[thread_count];
  for (size_t index = 0; index < thread_count; ++index) {
    pthread_create(&threads[index], NULL, generate_r, (void*)index);
  }

  for (size_t index = 0; index < thread_count; ++index) {
    pthread_join(threads[index], NULL);
  }

  for (int index = 0; index < count; ++index) {
    printf("%d\n", numbers[index]);
  }

  free(numbers);
  return EXIT_SUCCESS;
}

void* generate(void* data) {
  size_t rank = (size_t)data;
  for (int index = rank; index < count; index += thread_count) {
    numbers[index] = my_rand(min, max);
  }
  return NULL;
}

void* generate_r(void* data) {
  size_t rank = (size_t)data;
  my_rand_state_t local_state;
  my_seed_r(&local_state, clock() + rank); // Semilla única por hilo
  
  for (int index = rank; index < count; index += thread_count) {
    numbers[index] = my_rand_r(&local_state, min, max);
  }
  return NULL;
}