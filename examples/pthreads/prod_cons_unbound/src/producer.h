// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4

#ifndef PRODUCER_H
#define PRODUCER_H

//** @brief es el método que es usado de manera paralela por múltiples hilos.
// verifica que hay campo en el buffer para producir, y lo hace. Usa un mutex
// para sumarle al contador que mantiene la próxima unidad disponible.*/
void* produce(void* data);

#endif  // PRODUCER_H
