# Enunciado
Modifique la simulación de red para recibir la cantidad de productores en el segundo argumento de línea de comandos. Cree tantos productores como indica el usuario y todos producen en la misma cola. Utilice mecanismos de control de concurrencia para que los productores se repartan la carga de creación de mensajes de red.

# Comentarios
Usa un mutex para proteger la cantidad de paquetes pendientes por producir.