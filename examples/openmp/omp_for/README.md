# Enunciado
Utilice la tecnología OpenMP para repartir las iteraciones de un ciclo por contador (for). Aplique algoritmos de mapeo estático (bloque, cíclico) y dinámico (dinámico y guiado). Estudie los efectos. Recuerde utilizar buenas prácticas de programación.

# Observaciones
En el schedule (static) cada hilo recibe un bloque de 5 iteraciones. 
```
0 → 0–4  
1 → 5–9  
2 → 10–14  
3 → 15–19
```

En el schedule (static,1) "cíclico" las iteraciones se distribuyen de manera intercalada.

```
0 → 0–4  
1 → 5–9  
2 → 10–14  
3 → 15–19
```

En el schedule (dynamic,2) cada hilo toma 2 iteraciones después de haber terminado sus tareas anteriores. La asignación no es predecible: depende del orden en que los hilos terminan su trabajo.

En el schedule(guided,2) cada hilo toma bloques más grandes y luego van bajando el tamaño de los bloques que toman cuando terminan sus trabajo.