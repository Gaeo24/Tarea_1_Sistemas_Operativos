#include <parseador.h>

char *leer_linea(){
    char *linea = NULL;
    size_t buffersize = 0;

    if (getline(&linea, &buffersize, stdin) == -1){
        if (feof(stdin)){ //maneja EOF (ctrl+D)
            exit(EXIT_SUCCESS); 
        } else {
            perror("error: leer_linea");
            exit(EXIT_FAILURE);
        }
    }
    
    return linea;
}

char **parsear_linea(char *linea){
    int buffersize = TOKEN_BUFFER_SIZE;
    char *token;
    char **tokens = malloc(buffersize * sizeof(char*));

    token = strtok(linea, TOKEN_DELIMITERS);
    
    int indice = 0;
    while (token != NULL){
        tokens[indice] = token;
        indice++;

        if (indice >= buffersize) {
            buffersize += TOKEN_BUFFER_SIZE;
            tokens = realloc(tokens, buffersize * sizeof(char*));
        }

        token = strtok(NULL, TOKEN_DELIMITERS);
    }
    tokens[indice] = NULL;
    return tokens;
}