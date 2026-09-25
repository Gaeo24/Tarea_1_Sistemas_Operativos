#ifndef EJECUTAR
#define EJECUTAR

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int lanzar_proceso(char **tokens);
int contiene_pipe(char **tokens);
int ejecutar_pipeline(char **tokens);
#endif