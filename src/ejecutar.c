#include <ejecutar.h>
#include <redireccion.h>


//crea un proceso hijo para ejecutar el comando ingresado
int lanzar_proceso(char **tokens){
    pid_t pid, wpid;
    int status;

    //crea una copia del proceso actual
    pid = fork();
    if (pid == 0){
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
        //muestra un error si no se pudo crear el proceso hijo
        perror("error");
    } else {
        //el proceso padre espera a que termine el proceso hijo
        do {
            wpid=waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) &&  !WIFSIGNALED(status));

    return 1;
    }
}