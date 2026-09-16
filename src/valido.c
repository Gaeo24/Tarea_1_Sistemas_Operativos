#include <stdio.h>
#include <string.h>

void validar_entrada(char **tokens) {
    char *orden= tokens[0]; //entrada de la orden
    int accion= -1;//variable para determinar la acción a realizar
    char *ordenes[] = {"cd","exit","jobs","pmon"}; // lista de ordenes válidas 
    
    if (orden == NULL ) {
        fprintf(stderr, "Error: Entrada vacía o nula.\n");
        }
    else
    { //validar la orden segun la lista 
        int largo_orden=strlen(orden);
        if(largo_orden < 3) {
            if(orden[0]== 'c' && orden[1] == 'd') {
                accion = 0; // Acción para "cd"
            } 
        }
        else{
            for(int ii=1;ii<4;ii++){
                if(strcmp(orden,ordenes[ii])==0){
                    accion = ii; //si alguna orden es igual a al ade la lsita
                }
            }
            if (accion == -1) {
                fprintf(stderr, "Error: Orden no válida.\n");
            }
        }

        switch (accion)
        {
        case   0:
            if (tokens[1] == NULL) {
                //cd a home
            }
            else if (tokens[2] != NULL)
            {
                fprintf(stderr, "Error: Demasiados argumentos para 'cd'.\n");
            }
             
            else {
                // cd a Token[1]
            
            }
            //cd
            break;
        case   1:

            if (tokens[1] == NULL) {
                fprintf(stderr, "Error: 'exit' necesita un argumento.\n");
            }
            else if(tokens[2] != NULL) {
                fprintf(stderr, "Error: Demasiados argumentos para 'exit'.\n");
            }
            else {
                //exit con Token[1]
            }

            //exit
            break;
        case   2:
            if (tokens[1] != NULL) {
                fprintf(stderr, "Error: 'jobs' no acepta argumentos.\n");
            }else {
                //hacer jobs
            }
                
            break;
        case   3:
            if(tokens[1] == NULL) {
                fprintf(stderr, "Error: 'pmon' necesita un argumento.\n");
            }
            else if(tokens[2] != NULL) {
                fprintf(stderr, "Error: Demasiados argumentos para 'pmon'.\n");
            }
            else {
                //hacer pmon con Token[1]
            }
            //pmon
            break;
        default:
            break;
        }
   }
}