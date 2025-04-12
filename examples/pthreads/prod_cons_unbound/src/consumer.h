// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4

#ifndef CONSUMER_H
#define CONSUMER_H

//** @brief es el método que es usado de manera paralela por múltiples hilos.
// verifica si hay algo en el buffer para consumir, y lo hace. Usa un mutex
// para sumarle al contador que mantiene la cantidad de unidades consumidas.*/
void* consume(void* data);

#endif  // CONSUMER_H
