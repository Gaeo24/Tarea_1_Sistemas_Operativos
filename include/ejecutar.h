#ifndef EJECUTAR
#define EJECUTAR

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <shell.h>

int lanzar_proceso(char **tokens,int background, ShellState *shellState);
int contiene_pipe(char **tokens);
int ejecutar_pipeline(char **tokens);

#endif