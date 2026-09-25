cmake_minimum_required(VERSION 3.10.0)
project(tarea_so_1 VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

add_executable(tarea_so_1
	src/main.c
	src/parseador.c
	src/ejecutar.c
	src/ej_background.c
	src/comandos_internos.c
)
target_include_directories(tarea_so_1 PRIVATE include)
target_compile_definitions(tarea_so_1 PRIVATE _POSIX_C_SOURCE=200809L)

include(CTest)
enable_testing()

