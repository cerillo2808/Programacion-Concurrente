# Enunciado
Cree una nueva versión de su programa "hola mundo" donde cada thread imprime "Hello from secondary thread i of w", donde i es el número de thread y w la cantidad total de hilos creada. Está prohibido usar variables en el segmento de datos (globales, estáticas, externas). Cree un registro de memoria (estructura) privado para cada thread.

# Observaciones
- Cuando se ejecuta un programa, retorna un valor. Este valor es 0 cuando no hubo problemas, pero el valor puede ser cambiado a lo que sea. Diferentes errores dan diferentes valores. Como programadores tenemos control sobre lo que main devuelve.
- El paralelismo de datos es cuando un for crea hilos que realizan la misma tarea.
- Concurrencia de datos es cuando creo hilos que hacen otras tareas.
- La diferencia entre un array y un struct es que en el array tengo un segmento de memoria con datos del mismo tamaño/tipo, mientras que en el struct es un registro o una región continua de memoria.
- Calloc es más apropiado para pedir memoria para arreglos.
- Typedef lo que hace es reemplazar el tipo de dato que le sigue por un nombre. Ejemplo: typedef struct private_data, cada que el compilador vea private_data, va a saber que es un struct.
- La memoria debo eliminarla de manera inversa a la que se crean.