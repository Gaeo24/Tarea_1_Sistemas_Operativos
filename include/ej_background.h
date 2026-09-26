#ifndef EJ_BACKGROUND_H
#define EJ_BACKGROUND_H
#include <shell.h>
#include <sys/types.h>

int encontrar_background(char **tokens);
void quitar_background(char **tokens);
int configurar_sigchld(ShellState *shellState);
int agregar_job(ShellState *shellState, pid_t pid,  char **token);
void mostrar_job_terminado(ShellState *shellState);



#endif