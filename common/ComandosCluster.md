# Comandos para usar el clúster Poás

## Regla de oro
NO ejecutar cosas en la computadora principal. Usar la cola!!!

## Conectarse por ssh
`ssh -Y C38680@10.84.19.15`

Darle "yes" y luego escribir la contraseña.

## Opción 1: Clonar el repositorio
`git clone` dentro del clúster

## Opción 2: Copiar cosas de mi computadora al clúster
Es el secure copy. <br>
Un archivo: `scp nombre_archivo.c C38680@10.84.19.15:` <br>
Un directorio: `scp -r nombre_carpeta C38680@10.84.19.15:` <br>

Luego de eso va a pedir la clave de nuevo.

## Copiar cosas del clúster a mi computadora
`scp C38680@10.84.19.15:/~nombre_archivo .` <br>

El . es el directorio actual. El ~ es el home/usuario.

## Ver los módulos disponibles
`module avail` y los que ya tengan (L) es porque ya están pre-cargados. Para solo ver los que ya está pre-cargados, usar `module list`.

## Cargar y descargar módulo
`module load nombre/version` y `module unload nombre/version`, respectivamente.

## Ejecutar cosas en la cola
Ver cuáles hay disponibles con `scontrol show partition` <br>

Para ver cosas de la cola: `squeue` <br>

Meterse a batch-job-example-mpi para saber qué comandos hay y editar los nodos a utilizar. <br>

`sbatch batch-ejecutable` <br>

## Cancelar un trabajo
`scancel id_trabajo`, y el id se ve con squeue.