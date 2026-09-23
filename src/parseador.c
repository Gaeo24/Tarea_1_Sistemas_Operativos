#include <parseador.h>

//muestra el directorio actual como parte del prompt de la shell
void imprimir_prompt(){
    //getcwd reserva automaticamente un espacio del tamaño necesario
    char *cwd = getcwd(NULL, 0);

    if (cwd != NULL){
        //imprime el prompt y libera la memoria reservada por getcwd
        fprintf(stdout, "miShell:%s$ ", cwd);
        //asegura que el prompt aparezca inmediatamente en pantalla
        fflush(stdout);
        free(cwd);
    } else {
        //muestra un error si no se pudo obtener el directorio actual
        perror("error de getcwd()");
    }
}

//lee una linea completa desde la entrada estandar (stdin)
char *leer_linea(){
    char *linea = NULL;
    size_t buffersize = 0;

    if (getline(&linea, &buffersize, stdin) == -1){
        if (feof(stdin)){ //maneja EOF (ctrl+D)
            if (linea) free(linea);
            return strdup("exit 0"); //Cerramos limpiamente.
        } else {
            perror("error: leer_linea");
            if (linea) free(linea);
            return strdup("exit 1"); //Cerramos limpiamente.
        }
    }
    
    return linea;
}

//divide la linea ingresada en tokens separados por espacios
char **parsear_linea(char *linea){
    int buffersize = TOKEN_BUFFER_SIZE;
    char *token;
    char **tokens = malloc(buffersize * sizeof(char*));

    token = strtok(linea, TOKEN_DELIMITERS);
    
    //recorre la linea y guarda cada token en el arreglo
    int indice = 0;
    while (token != NULL){
        tokens[indice] = token;
        indice++;
        
        //aumenta el espacio si el arreglo se queda sin capacidad
        if (indice >= buffersize) {
            buffersize += TOKEN_BUFFER_SIZE;
            tokens = realloc(tokens, buffersize * sizeof(char*));
        }

        token = strtok(NULL, TOKEN_DELIMITERS);
    }

    //marca el final del arreglo tokens
    tokens[indice] = NULL;
    return tokens;
}