#include <shell.h>
#include <parseador.h>
#include <ejecutar.h>
#include <comandos_internos.h>
#include <ej_background.h>

static void ejecutar_entrada(char **tokens, ShellState *shellState){
    if (tokens[0] == NULL) {
        return;
    }

    if (contiene_pipe(tokens)){
        shellState->exit_status = ejecutar_pipeline(tokens);
        return;
    }

    if (!ejecutar_comandos_internos(tokens, shellState)){
        shellState->exit_status = lanzar_proceso(tokens);
    }
}
int main(int, char**){
    char *linea;
    char **tokens;
    int background;
    ShellState shellState;

    /* Inicializamos el estado de la shell */
    shellState.running = 1;
    shellState.exit_status = 0;
    shellState.jobs = NULL;
    shellState.capacidad_jobs = 0;
    shellState.siguiente_job_id = 1;
    /*config de sigchld*/
    if (configurar_sigchld(&shellState) == -1) {
        fprintf(stderr, "Error al configurar SIGCHLD\n");
        return EXIT_FAILURE;
    }

    /* Loop que se ejecuta hasta que alguna condicion de detencion se cumpla */
    do{
        mostrar_job_terminado(&shellState); /* muestra los jobs que terminaron */
        imprimir_prompt(); /* imprime prompt en consola */
        linea = leer_linea(); /* lee la linea desde consola */
        tokens = parsear_linea(linea); /* parsea la linea en argumentos */
      
        ejecutar_entrada(tokens, &shellState);
       
        if (tokens[0] != NULL){ /* Se verifica si no se escribió nada. */
            background = encontrar_background(tokens);
            if (background) {
                quitar_background(tokens); /* quita el & de la orden */
            }
            if (!ejecutar_comandos_internos(tokens, &shellState)) {
                shellState.running = lanzar_proceso(tokens, background, &shellState);
            }
        }
      
        free(tokens);
        free(linea); //libera memoria de linea y tokens en cada iteración
    } while (shellState.running);
    /* libera memoria de jobs */
    free(shellState.jobs);
    return shellState.exit_status;
}
