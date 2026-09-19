#ifndef SHEL_H
#define SHEL_H

#include <stdio.h>
#include <stdlib.h>

//Struct que concentra los datos de un job (tarea/proceso de la shell).
typedef struct {
    int id;             //Identificador para la shell.
    pid_t pid;          //ID real del sistema operativo.
    char comando[256];  //Nombre del comando.
    int activo;         //Estado del proceso. (Slot libre/ocupado)
} Job;

//Struct que concentra datos de la shell.
typedef struct {
    int running;            //Define si el programa continua o no.
    int exit_status;        //Almacena el estado de salida si se usó exit.
    Job *jobs;              //Arreglo de jobs.
    int capacidad_jobs;     //Memoria solicitada (Útil para recorrer *jobs).
} ShellState;

#endif