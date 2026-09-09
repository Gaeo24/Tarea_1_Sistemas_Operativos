cmake_minimum_required(VERSION 3.10.0)
project(tarea_so_1 VERSION 0.1.0 LANGUAGES C)

add_executable(tarea_so_1
	src/main.c
	src/parseador.c
)
target_include_directories(tarea_so_1 PRIVATE include)

include(CTest)
enable_testing()

