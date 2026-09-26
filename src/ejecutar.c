#include <ejecutar.h>
#include <ej_background.h>
#include <errno.h>
#include <redireccion.h>


/*crea un proceso hijo para ejecutar el comando ingresado*/
int lanzar_proceso(char **tokens,int background, ShellState *shellState){
    pid_t pid, wpid;
    int status;

    sigset_t mascara, mascara_anterior;
    
    /*bloquea sigchld para asegurar la integridad de la lista de jobs*/
    sigemptyset(&mascara);
    sigaddset(&mascara, SIGCHLD);
    sigprocmask(SIG_BLOCK, &mascara, &mascara_anterior);

    /* crea una copia del proceso actual */
    pid = fork();
    if (pid == 0){
        /* el proceso hijo reemplaza su programa por el comando indicado */
        /* desbloquea sigchld en el proceso hijo */
        sigprocmask(SIG_SETMASK, &mascara_anterior, NULL); 
        if(execvp(tokens[0], tokens) == -1){
        //guarda argumentos, sin operadores de redireccion
        char *arg[256];     

        //procesar redireccion devuelve la cantidad de argumentos, o -1 si ocurre un error
        if(procesar_redireccion(tokens, arg) <= 0){
            _exit(1);
        }

        //el proceso hijo reemplaza su programa por el comando indicado
        if(execvp(arg[0], arg) == -1){
            perror("error");
        }
        exit(EXIT_FAILURE);
    } else if (pid<0){
        /* muestra un error si no se pudo crear el proceso hijo */
        perror("error");
        return 0;
    } else {
        if (background) {

        int indice_job;
        indice_job = agregar_job(shellState, pid, tokens);
        if (indice_job == -1) {

            /*
             * Si no podemos registrar el job,
             * terminamos el proceso para no dejar
             * un hijo sin controlar.
             */
            kill(pid, SIGTERM);

            waitpid(pid, NULL, 0);

            sigprocmask(SIG_SETMASK, &mascara_anterior, NULL);

            return 0;
        }

        printf("[%d] %d\n", shellState->jobs[indice_job].id, pid);

        
        sigprocmask(SIG_SETMASK, &mascara_anterior, NULL);
        return 1;
    }

   
    do {
        wpid = waitpid(pid, &status, WUNTRACED);
    } while (
        wpid > 0 &&
        !WIFEXITED(status) &&
        !WIFSIGNALED(status)
    );

    if (wpid == -1 && errno != EINTR) {
        perror("waitpid");
    }

    sigprocmask(SIG_SETMASK, &mascara_anterior, NULL);

    return 1;
    }
}