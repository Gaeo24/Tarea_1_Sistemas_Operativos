#include <ejecutar.h>

//crea un proceso hijo para ejecutar el comando ingresado
int lanzar_proceso(char **tokens){
    pid_t pid, wpid;
    int status;

    //crea una copia del proceso actual
    pid = fork();
    if (pid == 0){
        //el proceso hijo reemplaza su programa por el comando indicado
        if(execvp(tokens[0], tokens) == -1){
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