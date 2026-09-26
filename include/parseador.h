#ifndef PARSEADOR
#define PARSEADOR

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TOKEN_BUFFER_SIZE 64
#define TOKEN_DELIMITERS " \t\r\n" /*espacio, tabulador y saltos de linea  */

void imprimir_prompt(void);
char *leer_linea(void);
char **parsear_linea(char *linea);

#endif