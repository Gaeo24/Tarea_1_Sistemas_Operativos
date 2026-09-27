## Estructura del proyecto 
### Carpeta include
Contiene archivos .h

### Carpeta src
 Contiene archivos .c

### Archivo makefile.mk
Se encarga de construir el proyecto y ejecutable. 

Se deben actualizar los sources cuando se agregan archivos .c nuevos.
## Comandos de build

### make -f makefile.mk

Este comando genera todos los archivos necesarios en la carpeta *./build-make*. 

El archivo *tarea_so_1.exe* se encuentra dentro de esta carpeta. *./build-make/tarea_so_1*

### make -f makefile.mk clean

Este comando limpia todos los archivos generados.