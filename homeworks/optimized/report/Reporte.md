# Reporte de optimizaciones

## Optimizaciones seriales

| Iter. | Etiqueta  | Duración (s) | _Speedup_ | Descripción corta                        |
|-------|-----------|--------------|-----------|------------------------------------------|
| 0     | Serial0   | 47.9300      | 1.00      | Versión serial inicial (Tarea01)         |
| 1     |           |              |           |                                          |
| 2     |           |              |           |                                          |

### Versión serial original (Tarea01)

> _Describa aquí qué aspectos tenía la versión serial no eficientes que podrían o fueron optimizados luego.  
> También aspectos que usted considera eficientes, los cuales explicarían por qué no amerita una optimización._

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
