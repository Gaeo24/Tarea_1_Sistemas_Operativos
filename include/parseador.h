#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOKEN_BUFFER_SIZE 64
#define TOKEN_DELIMITERS " "

char *leer_linea(void);
char **parsear_linea(char *linea);