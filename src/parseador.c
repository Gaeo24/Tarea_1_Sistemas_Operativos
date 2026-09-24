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

//agrega espacios alrededor de cada pipe para que strtok los detecte
static char *normalizar_linea(const char *linea){
    size_t capacidad = strlen(linea) * 3 + 1;
    char *resultado = malloc(capacidad);
    size_t indice = 0;

    if (resultado == NULL) {
        perror("error de malloc");
        exit(EXIT_FAILURE);
    }

    for (size_t i=0; linea[i] != '\0'; i++){
        if (linea[i] == '|'){
            resultado[indice++] = ' ';
            resultado[indice++] = '|';
            resultado[indice++] = ' ';
        } else {
            resultado[indice++] = linea[i];
        }
    }

    resultado[indice] = '\0';
    return resultado;
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
    char *linea_normalizada = normalizar_linea(linea);
    free(linea);

    return linea_normalizada;
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

            if (tokens == NULL){
                perror("error de realloc");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, TOKEN_DELIMITERS);
    }

    //marca el final del arreglo tokens
    tokens[indice] = NULL;
    return tokens;
}