#include "redireccion.h"

int procesar_redireccion(char **tokens, char **args){
    char *archivo_input = NULL;    
    char *archivo_output = NULL;
    //indica si la salida se agrega al final del archivo    
    int agregar_final = 0;
    int cantidad_args = 0;
    
    //separa operadores de redireccion
    for(int i = 0; tokens[i] != NULL; i++){
        if(strcmp(tokens[i], "<") == 0){
            if(tokens[i+1] == NULL){
                fprintf(stderr, "Falta archivo de entrada\n");
                return -1;
            }
            archivo_input = tokens[++i];
        }
        else if(strcmp(tokens[i], ">") == 0){
            if(tokens[i+1] == NULL){
                fprintf(stderr, "Falta archivo de salida\n");
                return -1;
            }
            archivo_output = tokens[++i];
            agregar_final = 0;
        }
        else if(strcmp(tokens[i], ">>") == 0){
            if(tokens[i+1] == NULL){
                fprintf(stderr, "Falta archivo para append\n");
                return -1;
            }
            archivo_output = tokens[++i];
            agregar_final = 1;
        }
        //guarda los argumentos que no sean operadores de redireccion
        else{
            args[cantidad_args++] = tokens[i];
        }
    }

    args[cantidad_args] = NULL;

    //ejecuta la redireccion 
    if(archivo_input != NULL){
        int fd = open(archivo_input, O_RDONLY);
        if (fd < 0) {
            perror("open");
            return -1;
        }

        if (dup2(fd, STDIN_FILENO) < 0) {
            perror("dup2");
            close(fd);
            return -1;
        }

        close(fd);
    }

    if(archivo_output != NULL){
        int fd = open(archivo_output, O_WRONLY | O_CREAT | (agregar_final ? O_APPEND : O_TRUNC), 0644);
        if (fd < 0) {
            perror("open");
            return -1;
        }

        if (dup2(fd, STDOUT_FILENO) < 0) {
            perror("dup2");
            close(fd);
            return -1;
        }

        close(fd);
    }

    return cantidad_args;
}