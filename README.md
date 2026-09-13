## Estructura del proyecto 
### Carpeta include
Contiene archivos .h

### Carpeta src
 Contiene archivos .c

### Archivo CMakeLists.txt
Se encarga de construir el proyecto y ejecutable. 

Se debe actualizar cuando se agregan archivos .c nuevos.
## Comando de build

cmake -S . -B build 

cmake --build build