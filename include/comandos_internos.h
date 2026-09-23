#ifndef COMANDOS_INTERNOS
#define COMANDOS_INTERNOS

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <shell.h>

int ejecutar_comandos_internos(char **tokens, ShellState *shellState);

#endif