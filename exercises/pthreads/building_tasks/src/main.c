// Copyright 2024 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
#define _XOPEN_SOURCE 500

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

#include "ethread.h"

// Índices para cada tarea
enum TaskIndex {
  WALLS = 0,              // 1.2 Obra gris
  EXTERIOR_PLUMBING,       // 2.1 Plomería exterior
  ROOF,                   // 2.3 Techo
  EXTERIOR_PAINTING,       // 3.1 Pintura exterior
  INTERIOR_PLUMBING,       // 3.2 Plomería interior
  ELECTRICAL_INSTALLATION,  // 3.3 Instalación eléctrica
  EXTERIOR_FINISHES,       // 4.1 Acabados exteriores
  INTERIOR_PAINTING,       // 4.3 Pintura interior
  FLOORS,                 // 5.3 Piso
  INTERIOR_FINISHES,       // 6.3 Acabados interiores
  NUM_TASKS
};

struct shared_data {
  useconds_t max_duration;  // microseconds
  // se hace una variable para cada acción para ver si ya ha sido terminada
  pthread_mutex_t mutex;
  int completed[NUM_TASKS];
};

int do_concurrent_tasks(struct shared_data* shared);
void* walls(void* data);
void* exterior_plumbing(void* data);
void* roof(void* data);
void* exterior_painting(void* data);
void* interior_plumbing(void* data);
void* electrical_installation(void* data);
void* exterior_finishes(void* data);
void* interior_painting(void* data);
void* floors(void* data);
void* interior_finishes(void* data);
void wait_for_dependencies(struct shared_data* shared, const int* deps,
                           int count);

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  struct shared_data* shared = (struct shared_data*)
      calloc(1, sizeof(struct shared_data));
  assert(shared);
  if (argc == 2 && sscanf(argv[1], "%u", &shared->max_duration) == 1 &&
      shared->max_duration) {
    srand48(time(NULL) + clock());
    do_concurrent_tasks(shared);
  } else {
    fprintf(stderr, "Ingrese máxima duración en microsegundos.\n");
    error = EXIT_FAILURE;
  }
  free(shared);
  return error;
}

//** @brief La función sirve para saber si la tarea del cual se depende ya está
//** terminada. Si no lo está, espera en la cola. */
void wait_for_dependencies(struct shared_data* shared, const int* deps,
                           int count) {
  int all_done = 0;
  while (!all_done) {
      pthread_mutex_lock(&shared->mutex);
      all_done = 1;
      for (int i = 0; i < count; ++i) {
          if (!shared->completed[deps[i]]) {
              all_done = 0;
              break;
          }
      }
      pthread_mutex_unlock(&shared->mutex);
      if (!all_done) usleep(1000);
  }
}

int do_concurrent_tasks(struct shared_data* shared) {
  struct thread_team* team = reserve_threads(11, shared);
  assert(team);
  add_thread(team, walls);
  add_thread(team, exterior_plumbing);
  add_thread(team, roof);
  add_thread(team, exterior_painting);
  add_thread(team, interior_plumbing);
  add_thread(team, electrical_installation);
  add_thread(team, exterior_finishes);
  add_thread(team, interior_painting);
  add_thread(team, floors);
  add_thread(team, interior_finishes);
  join_threads(team);

  return EXIT_SUCCESS;
}

void* walls(void* data) {
  struct shared_data* shared = (struct shared_data*) get_shared_data(data);
  puts("1.2 Obra gris empieza");
  assert(shared);
  usleep(lrand48() % shared->max_duration);

  pthread_mutex_lock(&shared->mutex);
  shared->completed[WALLS] = 1;
  pthread_mutex_unlock(&shared->mutex);

  puts("1.2 Obra gris termina");
  return NULL;
}

void* exterior_plumbing(void* data) {
  struct shared_data* shared = (struct shared_data*) get_shared_data(data);
  // esperar a que obra gris termine
  const int deps[] = {WALLS};
  wait_for_dependencies(shared, deps, 1);

  puts("2.1 Plomería exterior empieza");
  usleep(lrand48() % shared->max_duration);

  pthread_mutex_lock(&shared->mutex);
  shared->completed[EXTERIOR_PLUMBING] = 1;
  pthread_mutex_unlock(&shared->mutex);

  puts("2.1 Plomería exterior termina");
  return NULL;
}

void* roof(void* data) {
  struct shared_data* shared = (struct shared_data*) get_shared_data(data);
  // esperar a que obra gris termine
  const int deps[] = {WALLS};
  wait_for_dependencies(shared, deps, 1);

  puts("2.3 Techo empieza");
  usleep(lrand48() % shared->max_duration);

  pthread_mutex_lock(&shared->mutex);
  shared->completed[ROOF] = 1;
  pthread_mutex_unlock(&shared->mutex);

  puts("2.3 Techo termina");
  return NULL;
}

void* exterior_painting(void* data) {
  struct shared_data* shared = (struct shared_data*) get_shared_data(data);
  // esperar a que plomería exterior termine
  const int deps[] = {EXTERIOR_PLUMBING};
  wait_for_dependencies(shared, deps, 1);

  puts("3.1 Pintura exterior empieza");
  usleep(lrand48() % shared->max_duration);

  pthread_mutex_lock(&shared->mutex);
  shared->completed[EXTERIOR_PAINTING] = 1;
  pthread_mutex_unlock(&shared->mutex);

  puts("3.1 Pintura exterior termina");
  return NULL;
}

void* interior_plumbing(void* data) {
  struct shared_data* shared = (struct shared_data*) get_shared_data(data);
  // esperar a que pintura exterior termine
  const int deps[] = {EXTERIOR_PLUMBING};
  wait_for_dependencies(shared, deps, 1);

  puts("3.2 Plomería interior empieza");
  usleep(lrand48() % shared->max_duration);

  pthread_mutex_lock(&shared->mutex);
  shared->completed[INTERIOR_PLUMBING] = 1;
  pthread_mutex_unlock(&shared->mutex);

  puts("3.2 Plomería interior termina");
  return NULL;
}

void* electrical_installation(void* data) {
  struct shared_data* shared = (struct shared_data*) get_shared_data(data);
  // esperar a que techo termine
  const int deps[] = {ROOF};
  wait_for_dependencies(shared, deps, 1);

  puts("3.3 Instalación eléctrica empieza");
  usleep(lrand48() % shared->max_duration);

  pthread_mutex_lock(&shared->mutex);
  shared->completed[ELECTRICAL_INSTALLATION] = 1;
  pthread_mutex_unlock(&shared->mutex);

  puts("3.3 Instalación eléctrica termina");
  return NULL;
}

void* exterior_finishes(void* data) {
  struct shared_data* shared = (struct shared_data*) get_shared_data(data);
  // esperar a que pintura exterior termine
  const int deps[] = {EXTERIOR_PAINTING};
  wait_for_dependencies(shared, deps, 1);

  puts("4.1 Acabados exteriores empieza");
  usleep(lrand48() % shared->max_duration);

  pthread_mutex_lock(&shared->mutex);
  shared->completed[EXTERIOR_FINISHES] = 1;
  pthread_mutex_unlock(&shared->mutex);

  puts("4.1 Acabados exteriores termina");
  return NULL;
}

void* interior_painting(void* data) {
  struct shared_data* shared = (struct shared_data*) get_shared_data(data);
  // esperar a que plomería interior e instalación eléctrica terminen
  const int deps[] = {ELECTRICAL_INSTALLATION, INTERIOR_PLUMBING};
  wait_for_dependencies(shared, deps, 2);

  puts("4.3 Pintura interior empieza");
  usleep(lrand48() % shared->max_duration);

  pthread_mutex_lock(&shared->mutex);
  shared->completed[INTERIOR_PAINTING] = 1;
  pthread_mutex_unlock(&shared->mutex);

  puts("4.3 Pintura interior termina");
  return NULL;
}

void* floors(void* data) {
  struct shared_data* shared = (struct shared_data*) get_shared_data(data);
  // esperar a que pintura interior termine
  const int deps[] = {INTERIOR_PAINTING};
  wait_for_dependencies(shared, deps, 1);

  puts("5.3 Piso empieza");
  usleep(lrand48() % shared->max_duration);

  pthread_mutex_lock(&shared->mutex);
  shared->completed[FLOORS] = 1;
  pthread_mutex_unlock(&shared->mutex);

  puts("5.3 Piso termina");
  return NULL;
}

void* interior_finishes(void* data) {
  struct shared_data* shared = (struct shared_data*) get_shared_data(data);
  // esperar a que piso termine
  const int deps[] = {FLOORS};
  wait_for_dependencies(shared, deps, 1);

  puts("6.3 Acabados interiores empieza");
  usleep(lrand48() % shared->max_duration);

  pthread_mutex_lock(&shared->mutex);
  shared->completed[INTERIOR_FINISHES] = 1;
  pthread_mutex_unlock(&shared->mutex);

  puts("6.3 Acabados interiores termina"); //NOLINT
  return NULL;
}
