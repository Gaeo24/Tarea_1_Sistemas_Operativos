#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOKEN_BUFFER_SIZE 64
#define TOKEN_DELIMITERS " \t\r\n" //espacio, tab, return y enter

char *leer_linea(void);
char **parsear_linea(char *linea);