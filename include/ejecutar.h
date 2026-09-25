#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <shell.h>

#ifndef EJECUTAR_H
#define EJECUTAR_H


int lanzar_proceso(char **tokens,int background, ShellState *shellState);

#endif