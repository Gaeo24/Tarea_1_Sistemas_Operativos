#ifndef SHEL_H
#define SHEL_H

#include <stdio.h>
#include <stdlib.h>

//Struct que concentra los datos de un job (proceso o grupo de procesos asociados a la shell)
typedef struct {
    int id;
    pid_t pid;
    char comando[256];
    int activo;
} Job;

//Struct que concentra datos de la shell
typedef struct {
    int running;          //Define si el programa continua o no.
    int exit_status;      //Almacena el estado de salida si se usó exit.
    Job *jobs;          //
    int capacidad;      // 
    int contador_jobs;
} ShellState;

#endif