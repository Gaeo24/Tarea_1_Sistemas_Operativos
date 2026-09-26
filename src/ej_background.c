#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ej_background.h>

static ShellState *estado_shell = NULL;


int encontrar_background(char **tokens){ /* ve si hay un & en la orden */
    int i= 0;
    while(tokens[i]!=NULL){
        if(strcmp(tokens[i], "&")==0){
            return 1; /* si sale 1 se ejecuta en background */
        }
        i++;
    }
    return 0; /* si es 0 se ejecuta normal */
}

void  quitar_background(char **tokens){ /* quita el & de la orden */
    int i= 0;
    while(tokens[i]!=NULL){
        if(strcmp(tokens[i], "&")==0){
            tokens[i]=NULL; /* se pone NULL para que no se ejecute el & */
            return;
        }
        i++;
    }
}

/* Se encarga de manejar la señal SIGCHLD, recoje los hijos cuando terminan(osea cambian de estado) */
static void manejar_sigchld(int signal){
    int status;
    pid_t pid;
    int i;

    (void)signal;
    if (estado_shell == NULL) {
        return;
    }

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {

        if (estado_shell == NULL) {
            continue;
        }

        /*
         * Buscamos a que job pertenecia el PID terminado.
         */
        for (i = 0; i < estado_shell->capacidad_jobs; i++) {

            if (estado_shell->jobs[i].activo &&
                estado_shell->jobs[i].pid == pid) {

                estado_shell->jobs[i].activo = 0;
                estado_shell->jobs[i].pendiente = 1;

                break;
            }
        }
    }
}


/* Configura el manejador de la señal SIGCHLD para la shell (matar zombies) */

int configurar_sigchld(ShellState *shellState)
{
    struct sigaction sa;

    estado_shell = shellState;

    memset(&sa, 0, sizeof(sa));

    sa.sa_handler = manejar_sigchld;

    if (sigemptyset(&sa.sa_mask) == -1) {
        perror("sigemptyset");
        return -1;
    }

    
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;

    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        return -1;
    }

    return 0;
}

int agregar_job(ShellState *shellState,pid_t pid,char **tokens){
    int i;
    int nueva_capacidad;
    Job *nuevos_jobs;
    Job *job;
    size_t usado;
    int cantidad;

    /* buscamos una ranura libre */
    for (i = 0; i < shellState->capacidad_jobs; i++) {
        if (!shellState->jobs[i].activo &&
            !shellState->jobs[i].pendiente) {
            break;
        }
    }

    /* Si no hay espacio, aumentamos la tabla*/
    if (i == shellState->capacidad_jobs) {

        if (shellState->capacidad_jobs == 0) {
            nueva_capacidad = 4;
        } else {
            nueva_capacidad = shellState->capacidad_jobs * 2;
        }

        nuevos_jobs = realloc(
            shellState->jobs,
            nueva_capacidad * sizeof(Job)
        );

        if (nuevos_jobs == NULL) {
            perror("realloc");
            return -1;
        }

        /* Inicializamos las nuevas ranuras */
        for (int j = shellState->capacidad_jobs;
             j < nueva_capacidad;
             j++) {

            nuevos_jobs[j].activo = 0;
            nuevos_jobs[j].pendiente = 0;
            nuevos_jobs[j].pid = 0;
            nuevos_jobs[j].id = 0;
            nuevos_jobs[j].comando[0] = '\0';
        }

        shellState->jobs = nuevos_jobs;
        shellState->capacidad_jobs = nueva_capacidad;
    }

    job = &shellState->jobs[i];

    job->id = shellState->siguiente_job_id;
    shellState->siguiente_job_id++;

    job->pid = pid;
    job->activo = 1;
    job->pendiente = 0;
    usado = 0;

    for (cantidad = 0;
         tokens[cantidad] != NULL && usado < sizeof(job->comando) - 1;
         cantidad++) {

        int escrito;

        escrito = snprintf(
            job->comando + usado,
            sizeof(job->comando) - usado,
            "%s%s",
            cantidad == 0 ? "" : " ",
            tokens[cantidad]
        );

        if (escrito < 0) {
            job->comando[0] = '\0';
            break;
        }

        if ((size_t)escrito >= sizeof(job->comando) - usado) {
            usado = sizeof(job->comando) - 1;
            break;
        }

        usado += (size_t)escrito;
    }

    job->comando[sizeof(job->comando) - 1] = '\0';

    return i;
}

void mostrar_job_terminado(ShellState *shellState)
{
    sigset_t mascara;
    sigset_t anterior;
    int i;

    sigemptyset(&mascara);
    sigaddset(&mascara, SIGCHLD);

    /* Bloqueamos SIGCHLD para evitar que se modifique la lista de jobs mientras la recorremos. */  
    sigprocmask(SIG_BLOCK, &mascara, &anterior);

    for (i = 0; i < shellState->capacidad_jobs; i++) {

        if (shellState->jobs[i].pendiente) {

            printf(
                "[%d]+ Done %s\n",
                shellState->jobs[i].id,
                shellState->jobs[i].comando
            );

            shellState->jobs[i].pendiente = 0;
        }
    }

    sigprocmask(SIG_SETMASK, &anterior, NULL);
}





