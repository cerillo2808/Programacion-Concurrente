# Enunciado
Modifique su solución a [hello_order_busywait](../hello_order_busywait/) para implementar una carrera de hilos de ejecución. Cada vez que un hilo llega a la meta, lo avisa en la salida estándar, como ocurre en la siguiente corrida hipotética:

```
Thread 0/4: I arrived at position 1
Thread 2/4: I arrived at position 2
Thread 1/4: I arrived at position 3
Thread 3/4: I arrived at position 4
```

Note que las posiciones tienen que aparecer en orden, mientras que el número de cada hilo es indeterminístico. Utilice algún mecanismo de control de concurrencia para que el reclamo de la posición sea el correcto.

# Observaciones
Cuando se utiliza condición de carrera, o sea, hacer que cada hilo modifique la variable compartida de posición, puede pasar que hay varios hilos que imprimen la misma posición.

Para verificar si hay condiciones de carrera, usar tsan.

La manera correcta para controlar la concurrencia en este ejemplo es el mutex. Hacer que la modificación de la variable posición sólo pueda ser modificada por un hilo a la vez.

