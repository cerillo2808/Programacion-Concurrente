# Enunciado

Haga el mínimo de modificaciones a la siguiente implementación del generador lineal congruencial de números pseudoaleatorios para que sea:

1. Thread-safe

2. Reentrante y thread-safe

# Modificaciones

- Thread-safe: Agrega un mutex para sincronizar el acceso al estado global.
- Reentrante: Elimina el estado global y usa una estructura que se pasa como parámetro, permitiendo que cada hilo tenga su propio estado.