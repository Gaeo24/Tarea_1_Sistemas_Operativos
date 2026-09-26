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

int contiene_pipe(char **tokens){
    for (int indice = 0; tokens[indice] != NULL; indice++){
        if (strcmp(tokens[indice], "|") == 0){
            return 1;
        }
    }

    return 0;
}

int ejecutar_pipeline(char **tokens){
    char **comandos[256];
    pid_t procesos[256];

    int cantidad_comandos = 1;
    int cantidad_tokens = 0;
    int entrada_anterior = -1;

    //cuenta todos los tokens antess de reemplazar los pipes
    while (tokens[cantidad_tokens] != NULL){
        cantidad_tokens++;
    }

    comandos[0] = tokens;

    for (int i = 0; i < cantidad_tokens; i++){
        if (strcmp(tokens[i], "|") == 0){
            //verifica que el pipe tenga comandos a ambos lados
            if (i == 0 || i + 1 == cantidad_tokens || tokens[i+1] == NULL ||
                strcmp(tokens[i + 1], "|") == 0 || 
                (i > 0 && strcmp(tokens[i - 1], "|") == 0)){
                fprintf(stderr, "error: pipe mal colocado");
                return 1;
            }

            tokens[i] = NULL;
            comandos[cantidad_comandos++] = &tokens[i+1];
        }
    }

    for (int i = 0; i < cantidad_comandos; i++){
        int descriptores[2] = {-1, -1};

        //todos los comandos excepto el ultimo deben crear un pipe
        if (i < cantidad_comandos - 1){
            if(pipe(descriptores) == -1){
                perror("error de pipe");
                return 1;
            }
        }

        procesos[i] = fork();

        if (procesos[i] == -1){
            perror("error de fork");
            return 1;
        }

        if (procesos[i] == 0){
            char *argumentos[256];

            //recibe la salida del comando anterior
            if (entrada_anterior != -1){
                if (dup2(entrada_anterior, STDIN_FILENO) == -1){
                    perror("error de dup2");
                    _exit(EXIT_FAILURE);
                }
            }

            //envia la salida al siguiente comando
            if (i < cantidad_comandos - 1){
                if (dup2(descriptores[1], STDOUT_FILENO) == -1){
                    perror("error de dup2");
                    _exit(EXIT_FAILURE);
                }
            }

            //cierra descriptores que ya fueron duplicados
            if (entrada_anterior != -1){
                close(entrada_anterior);
            }

            if (descriptores[0] != -1){
                close(descriptores[0]);
            }

            if (descriptores[1] != -1){
                close(descriptores[1]);
            }

            //procesa <, > y >> dentro del proceso correspondiente
            if (procesar_redireccion(comandos[i], argumentos) <= 0){
                _exit(EXIT_FAILURE);
            }

            if (execvp(argumentos[0], argumentos) == -1){
                perror("error de execvp");
                _exit(EXIT_FAILURE);
            }
        }

        //el padre cierra extremos innecesarios
        if (entrada_anterior != -1){
                close(entrada_anterior);
        }

        if (descriptores[1] !=-1){
            close(descriptores[1]);
        }
        //el siguiente proceso lee desde este descriptor
        entrada_anterior = descriptores[0];
    }

    //el padre cierra su ultima entrada pendiente
    if (entrada_anterior != -1){
        close(entrada_anterior);
    }

    //espera a que terminen todos los comandos
    for (int i = 0; i < cantidad_comandos; i++){
        waitpid(procesos[i], NULL, 0);
    }
    return 1;
}