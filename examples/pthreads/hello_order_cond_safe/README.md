# Enunciado
Al igual que [hello_order_semaphor](../hello_order_semaphor/), haga que los threads saluden siempre en orden. Es decir, si se crean w threads, la salida sea siempre en orden

Hello from thread 0 of w
Hello from thread 1 of w
Hello from thread 2 of w
...
Hello from thread w of w
Evite (si es posible) el control de concurrencia, es decir, trate de usar [seguridad condicional](../../../README.md/#12-seguridad-condicional).

# Observaciones
En vez de hacer que los hilos traten de accesar a la misma memoria, se hace que los hilos accedan a un índice personal de un arreglo el cual pueden editar. No es una condición de carrera porque ya no se cumple con memoria compartida.