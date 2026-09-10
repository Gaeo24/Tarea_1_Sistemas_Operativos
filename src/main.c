#include <shell.h>
#include <parseador.h>
#include <ejecutar.h>


int main(int, char**){
    char *linea;
    char **tokens;
    int status = 1;

    //loop que se ejecuta hasta que alguna condicion de detencion se cumpla
    do{
        imprimir_prompt(); //imprime prompt en consola, hay que cambiarlo por una función que cumpla los requisitos
        linea = leer_linea(); //lee la linea desde consola
        tokens = parsear_linea(linea); //parsea la linea en argumentos dentro de un arreglo
        status = lanzar_proceso(tokens); //ejecuta el comando ingresado
        
        free(linea); //libera memoria de linea y tokens en cada iteración
        free(tokens);
    } while (status);

    return EXIT_SUCCESS;
}
