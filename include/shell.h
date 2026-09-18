#ifndef SHEL_H
#define SHEL_H

#include <stdio.h>
#include <stdlib.h>

//Struct que concentra datos de la shell
typedef struct {
    int running;          //Define si el programa continua o no.
    int exit_status;      //Almacena el estado de salida si se usó exit.
    // ListaJobs *jobs; //Se añadirá después...
} ShellState;

#endif