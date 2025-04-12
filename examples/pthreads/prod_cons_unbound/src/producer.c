// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4

#include <stdio.h>

#include "common.h"
#include "producer.h"

void* produce(void* data) {
  // const private_data_t* private_data = (private_data_t*)data;
  // simulation funciona como la memoria compartida
  simulation_t* simulation = (simulation_t*)data;

  while (true) {
    // declare my_unit := 0
    size_t my_unit = 0;
    // lock(can_access_next_unit)
    pthread_mutex_lock(&simulation->can_access_next_unit);
    // If is there pending work, take a unit for producing
    if (simulation->next_unit < simulation->unit_count) {
      my_unit = ++simulation->next_unit;
    } else {
      // unlock(can_access_next_unit)
      pthread_mutex_unlock(&simulation->can_access_next_unit);
      // break while
      break;
    }
    // unlock(can_access_next_unit)
    pthread_mutex_unlock(&simulation->can_access_next_unit);

    // usa un random para saber cuánto es el delay
    usleep(1000 * random_between(simulation->producer_min_delay
      , simulation->producer_max_delay));
    // pone lo producido en la cola
    queue_enqueue(&simulation->queue, my_unit);
    printf("Produced %zu\n", my_unit);

    // signal(can_consume)
    sem_post(&simulation->can_consume);
  }

  return NULL;
}
