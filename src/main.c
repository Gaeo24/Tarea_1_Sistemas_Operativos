#include <shell.h>
#include <parseador.h>
#include <ejecutar.h>
#include <comandos_internos.h>

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

    //Inicializamos el estado de la shell
    ShellState shellState = {.running = 1, .exit_status = 0};

    //loop que se ejecuta hasta que alguna condicion de detencion se cumpla
    do{
        imprimir_prompt(); //imprime prompt en consola, hay que cambiarlo por una función que cumpla los requisitos
        linea = leer_linea(); //lee la linea desde consola
        tokens = parsear_linea(linea); //parsea la linea en argumentos dentro de un arreglo
        
        ejecutar_entrada(tokens, &shellState);

        free(tokens);
        free(linea); //libera memoria de linea y tokens en cada iteración
    } while (shellState.running);

    return shellState.exit_status;
}
