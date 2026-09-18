#include <shell.h>
#include <parseador.h>
#include <ejecutar.h>
#include <comandos_internos.h>


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
       
        if (tokens[0] != NULL){//Se verifica si no se escribió nada.
            if (!ejecutar_comando_internos(tokens, &shellState)) {
            shellState.running = lanzar_proceso(tokens); //ejecuta el comando ingresado
            }
        }

        free(linea); //libera memoria de linea y tokens en cada iteración
        free(tokens);
    } while (shellState.running);

    return shellState.exit_status;
}
