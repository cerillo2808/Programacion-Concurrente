# Enunciado
Al igual que [hello_order_busywait](../hello_order_busywait/), haga que los threads saluden siempre en orden. Es decir, si se crean w threads, la salida sea siempre en orden.

````
Hello from thread 0 of w
Hello from thread 1 of w
Hello from thread 2 of w
...
Hello from thread w of w
````

Utilice una colección de semáforos como mecanismo de sincronización (control de concurrencia).

# Observaciones
Se usa un arreglo de semáforos para controlar los múltiples hilos. Se pone un semáforo antes y después de la impresión, para que se impriman en orden. Después de a impresión, se incrementa el semáforo para permitir que el próximo hilo continúe.