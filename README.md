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
## Sintaxis soportada

La shell acepta los siguiente tipos de comandos:

### Comandos simples

```bash
ls 
pwd
echo hola
whoami
```
### Cambio de directorio

```bash
cd
cd /tmp
cd /home/usuario
```
### Procesos en segundo plano

```bash
sleep 10 &
find / -name "*.log" &
```

### Jobs

```bash
jobs
```
### Pipelines de longitud variable
La shell soporta pipelines con más de dos comandos:

```bash
ls | sort | wc -l
cat entrada.txt | grep error | cut -d: -f1
```
### Redirección de entrada

```bash
cat < archivo.txt
wc -l < archivo.txt
```

### Redirección de salida

```bash
echo hola > salida.txt
ls > listado.txt
```
### Redirección append

```bash
echo hola >> salida.txt
date >> log.txt
```
### Combinación de tuberías y redirecciones

```bash
cat < entrada.txt | wc -l > resultado.txt
```

### Salir de la shell

```bash
exit
exit 0
exit 1
```

### Monitor de procesos

```bash
pmon
pmon 2
```
## Ejemplo de ejecución

El siguiente ejemplo muestra las funcionalidades principales:
```bash
$ ./build-make/tarea_so_1
miShell:/home/usuario/tareas_so$ pwd
/home/usuario/tareas_so

miShell:/home/usuario/tareas_so$ echo "hola mundo" > salida.txt
miShell:/home/usuario/tareas_so$ cat salida.txt
"hola mundo"

miShell:/home/usuario/tareas_so$ sleep 15 &
[1] 18421

miShell:/home/usuario/tareas_so$ jobs
[1]    PID: 18421    Estado: Ejecutando     sleep 15

miShell:/home/usuario/tareas_so$ cat salida.txt | wc -l
1

miShell:/home/usuario/tareas_so$ pmon 2
Refrescando pmon cada 2 segundos... (Ctrl+C para salir)
PID     | Comando     | Estado    | %CPU | RSS (KB)
18421   | sleep 15    | durmiendo | 0.0  | 1000

^C
Saliendo del monitor pmon

miShell:/home/usuario/tareas_so$ exit
```

### Qué hace este ejemplo

- `pwd`: muestra el directorio actual.
- `echo ... > salida.txt`: redirección de salida.
- `cat salida.txt`: lectura del archivo.
- `sleep 15 &`: proceso en segundo plano.
- `jobs`: lista jobs activos.
- `cat salida.txt | wc -l` pipeline.
- `pmon 2`: monitor de procesos.
- `Ctrl+C`: sale del monitor sin cerrar la shell.
- `exit`: termina la shell.

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
