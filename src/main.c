#include <shell.h>
#include <parseador.h>


int main(int, char**){
    char *linea;
    char **tokens;
    int status = 1;

    //loop que se ejecuta hasta que alguna condicion de detencion se cumpla
    do{
        printf("> "); //imprime prompt en consola, hay que cambiarlo por una función que cumpla los requisitos
        linea = leer_linea(); //lee la linea desde consola
        tokens = parsear_linea(linea); //parsea la linea en argumentos dentro de un arreglo
        //status = ejecutar(args);
        
        //for loop que imprime en consola el mensaje ingresado dividido en tokens
        //esto es solo para verificar el funcionamiento de leer_linea y parsear_linea
        for (int i = 0; tokens[i] != NULL; i++) {
            fprintf(stdout, "token[%d] = %s\n", i, tokens[i]);
        }

        free(linea); //libera memoria de linea y tokens en cada iteración
        free(tokens);
    } while (status);

    return EXIT_SUCCESS;
}
