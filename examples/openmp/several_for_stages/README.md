# Enunciado

En el ejemplo several_for hay tres etapas (stages) de secciones omp for. Mofique este código para que el usuario pueda indicar la cantidad de etapas como tercer argumento de línea de comandos. Elimine el código redundante de la segunda y tercera etapa. Haga que su solución repita el código de una etapa, tantas veces como lo haya indicado el usuario. Asegúrese en cada etapa de repartir las iteraciones indicadas por el usuario en el segundo argumento de línea de comandos.

No debe crear ni destruir hilos de ejecución entre una etapa y otra. Todos los hilos deben ser creados antes de iniciar la primera etapa, y destruidos una vez que la última etapa ha sido ejecutada. Las salidas de una etapa no deben mezclarse con las salidas de la siguiente.

# Corrida en el clúster
![alt text](image.png)
