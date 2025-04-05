# Enunciado
Ejecute al menos tres veces los códigos de Ejemplo 7 (imprimir en orden con semáforos) y Ejemplo 8 (imprimir en orden con seguridad condicional) con tantos hilos como su sistema operativo permite.

Anote las tres duraciones de cada solución. Tome la mayor duración de las tres corridas de cada versión. Agregue los resultados a sus cuadros y gráficos del Ejercicio 15 [delayed_busy_wait].

Si en un proyecto con sus clientes usted tuviere que implementar una de las cuatro soluciones para hacer un trabajo en orden ¿Cuál solución escogería? Agregue además un párrafo a la discusión de Ejercicio 15 [delayed_busy_wait] explicando la potencial causa de este comportamiento.

# Datos
| Ejecucion | Semáforos | Seguridad condicional |
| --- | --- | --- |
| 1 | 0.001166734s | 0.000589417s
| 2 | 0.001130434s | 0.000549908s
| 3 | 0.001122001s | 0.000650558s

# Gráfica
[Gráfica original delayed_busy_wait](../delayed_busy_wait/README.md)

![alt text](<Gráfico comparación sem_vs_cond.png>)

# Observaciones

Se puede observar con la gráfica que el uso de semáforos no difiere mucho de aquella en la espera activa. Sin embargo, sí hay una gran diferencia de tiempo cuando se usa la seguridad condicional. 

Si estuviera implementando un proyecto, usaría la seguridad condicional porque es la más rápida, y si está bien implementada, no causa condiciones de carrera.

La razón por la que la seguridad condicional es la más rápida es porque no crea cuellos de botella al tratar de detener la concurrencia. Más bien, deja que la concurrencia siga su curso, y lo que hace es limitar la memoria a la que pueden accesar los hilos.