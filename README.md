# miShell

Shell interactiva escrita en C para sistemas Linux.

El proyecto implementa ejecución de comandos externos, comandos internos,
procesos en segundo plano, jobs, tuberías, redirecciones y un monitor básico
de procesos.

## Funcionalidades

- Ejecución de comandos externos mediante `execvp`.
- Comandos internos:
    - `cd`
    - `exit`
    - `jobs`
    - `pmon`
- Ejecución en segundo plano mediante `&`.
- Administración de jobs activos.
- Tuberías mediante `|`.
- Redirección de entrada mediante `<`.
- Redirección de salida mediante `>`.
- Redirección de salida en modo append mediante `>>`.
- Manejo de señales `SIGINT`, `SIGQUIT` y `SIGCHLD`.
- Monitorización de procesos con PID, estado, CPU y memoria RSS.

## Requisitos

- Sistema operativo Linux o compatible con POSIX.
- Compilador C, por ejemplo GCC o Clang.
- GNU Make.

En Debian o Ubuntu, las dependencias pueden instalarse con:

```bash
sudo apt update
sudo apt install build-essential
```
## Compilación desde cero

Clona el repositorio y accede a su directorio:

```bash
git clone https://github.com/Gaeo24/Tarea_1_Sistemas_Operativos.git
cd Tarea_1_Sistemas_Operativos
```
Compila el proyecto usando el Makefile incluido:
```bash
make -f makefile.mk
```
El ejecutable se generará en:
`build-make/tarea_so_1`

## Ejecución

Inicia la shell con:
```bash
./build-make/tarea_so_1
```
El programa mostrará un prompt similar a:
```bash
miShell:/ruta/actual$
```
Para finalizar la shell:
```bash
exit
```
## Ejemplo completo de uso

El siguiente ejemplo muestra las funcionalidades principales:
## Estructura del proyecto

```text
.
├── include/                # Archivos de cabecera
│   ├── comandos_internos.h
│   ├── ej_background.h
│   ├── ejecutar.h
│   ├── parseador.h
│   ├── pmon.h
│   ├── redireccion.h
│   ├── senales.h
│   └── shell.h
├── src/                    # Código fuente en C
│   ├── comandos_internos.c # cd, exit, jobs y pmon
│   ├── ej_background.c     # Procesos en segundo plano y jobs
│   ├── ejecutar.c          # Ejecución y tuberías
│   ├── main.c              # Punto de entrada
│   ├── parseador.c         # Lectura y análisis de comandos
│   ├── pmon.c              # Monitor de procesos
│   ├── redireccion.c       # Redirecciones <, > y >>
│   └── senales.c           # Manejo de señales
├── makefile.mk             # Reglas de compilación
└── README.md               # Documentación del proyecto
```
El Makefile genera los archivos intermedios y el ejecutable dentro de `build-make/`.

## Limpieza

Para eliminar los archivos generados durante la compilación:
```bash
make -f makefile.mk clean
```

## Limitaciones conocidas

- El parser utiliza espacios como separadores.
- No se admiten comillas para agrupar argumentos.
