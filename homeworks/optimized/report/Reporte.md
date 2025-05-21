# Reporte de optimizaciones

Las duraciones reportadas fueron la más rápida de tres ejecuciones diferentes en la misma máquina (clúster Poás) y el mismo caso de prueba (job001b). Las salidas de perf están en en el archivo [perf.txt](../auto_files/perf.txt). Otros archivos .out de corridas interesantes en kcachegrind se encuentran en la carpeta [auto_files](../auto_files/). Para correrlos, ejecute en su máquina `kcachegrind nombre.out`, sin embargo, no es muy necesario ya que para efectos del presente reporte, se adjuntan pantallazos relevantes que se encuentran en la carpeta [img](../img/).

## Optimizaciones seriales

| Iter. | Etiqueta  | Duración (s) | _Speedup_ | Descripción corta                        |
|-------|-----------|--------------|-----------|------------------------------------------|
| 0     | Serial0   | 47.629947625 | 1.00      | Versión serial inicial (Tarea01)         |
| -     |    | 49.283765940 | 0.96      | Usa el path completo, no el relativo     |
| 2     |           |              |           |                                          |

### Versión serial original (Tarea01)

Lo que hace el programa en su forma serial es leer el archivo job###.txt, crear el objeto plate, y simular la transferencia de calor por cada plate creado. La simulación se ejecuta celda por celda, lo cual ralentiza mucho el tiempo de ejecución si es una matriz grande. Luego, por ser serial, tiene que repetir todo el proceso anteriormente mencionado por cada línea que existe del job###.txt.

Un aspecto a optimizar podría ser disminuir la cantidad de celdas a recorrer en la matriz de la simulación, ya que si la celda es un borde, puede ser ignorada porque su temperatura no cambia debido a que se le inyecta calor. En esta versión, la simulación recorre absolutamente todas las celdas de la matriz, bordes incluidos, y cuando verifica que es un borde, la ignora.

Perf para el serial en job001b
![alt text](../img/image.png)
Kcachegrind para el serial job001b
[link](../auto_files/callgrind.out.3136474)

Según kcachegrind, el proceso que dura casi el 100% es el run, y en el run reporta que el proceso que dura más es la simulación a la hora de copiar la matriz.
![alt text](../img/kcachegrind%20serial%20main.png)
![alt text](../img/kchachegrind%20serial%20run.png)


### Iteración 1

> _Para cada iteración del ciclo de optimización, cree una subsección como esta.  
> Resuma en máximo un párrafo en qué consiste la optimización.  
> Su objetivo es que quien lea este documento entienda claramente qué se intentó mejorar.  
> Provea trocitos de código fuente o pseudocódigo que ayude a entender la optimización potencial.  
> Indique si el intento logró incrementar el desempeño.  
> En caso de no lograrlo, conjeture la causa.  
> Eso ayudaría a ahorrar tiempo a otras personas que tengan la misma inquietud._

Una solución al recorrido a todas las celdas de la matriz durante la simulación, podría ser darle un rango a la matriz para que sólo recorra "no_bordes". Para plates pequeños, no va a bajar mucho el tiempo de ejecución, de lo contrario, podría incluso ser más lento si la lógica para disminuir el rango a recorrer dura más que simplemente recorrer la celda borde e ignorarla. Pero, para matrices más grandes, donde hay una cantidad considerable de celdas borde, sí podría disminuir el tiempo de ejecución. En general, sí es una solución más eficiente ya que entre más grande se haga el problema, más tiempo ahorra.

Para implementarlo, 

## Optimizaciones concurrentes

| Iter. | Etiqueta  | Duración (s) | _Speedup_ | Eficiencia | Descripción corta                      |
|-------|-----------|--------------|-----------|------------|----------------------------------------|
| -     | SerialI   |              | 1.00      | 1.00       | Versión serial final                   |
| 1     | Conc0     | 214.5283 |           |            | Versión concurrente inicial (Tarea02) |
| 2     |           |              |           |            |                                        |
| 3     |           |              |           |            | 

Perf para versión concurrente (tarea 2) en job001b
![alt text](../img/image2.png)

Kcachegrind para el concurrente (tarea 2) en job001b
[link](../auto_files/callgrind.out.2209469)

Se puede ver del reporte que lo que más se dura haciendo es creando los hilos por cada iteración de la simulación.
![alt text](../img/kcachegrind%20pthread%208%20hilos.png)

Con un sólo hilo el reporte de kcachegrind da casi igual. Sin embargo, se le corre perf para sacar el speedup.

Versión concurrente final             

### Versión concurrente inicial (Tarea02)

> _Describa aquí qué aspectos tenía la versión concurrente (Tarea02) no eficientes que podrían o fueron optimizados luego.  
> También aspectos que usted considera eficientes, los cuales explicarían por qué no amerita una optimización._

Lo que podría ser optimizado es asignar un hilo por cada línea del job###.txt e ir procesando los plates de manera concurrente, sin embargo, podría no funcionar tan bien porque hay plates más grandes que otros, causando que algunos hilos terminen su ejecución temprano, sean desperdiciados y un solo hilo sea el encargado de ejecutar un plate grande.

### Iteración 1

> _Para cada iteración del ciclo de optimización, cree una subsección como esta.  
> Resuma en máximo un párrafo en qué consiste la optimización.  
> Su objetivo es que quien lea este documento entienda claramente qué se intentó mejorar.  
> Provea trocitos de código fuente o pseudocódigo que ayude a entender la optimización potencial.  
> Indique si el intento logró incrementar el desempeño.
> En caso de no lograrlo, conjeture la causa.  
> Eso ayudaría a ahorrar tiempo a otras personas que tengan la misma inquietud._

### Comparación de optimizaciones

_(pendiente)_

### Comparación del grado de concurrencia

_(pendiente)_
