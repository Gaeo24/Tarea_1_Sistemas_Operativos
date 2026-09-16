#include <comandos_internos.h>

//Comando cd
static void ejecutar_cd(char **args) {
    char *dir = args[1] != NULL ? args[1] : getenv("HOME");//Verifica argumento luego de cd
    //Si no está definido "HOME" lanza error.
    if (dir == NULL) fprintf(stderr, "miShell: cd: HOME no definido\n");
    //Se intenta cambiar de directorio, si no es posible, lanza error.
    else if (chdir(dir) != 0) perror("miShell: cd");
}

//Comando exit
static void ejecutar_exit(char **args) {
    //Antes de cerrar la shell, se guarda estado si el usuario lo ingresó. Por defecto 0.
    int status = args[1] != NULL ? atoi(args[1]) : 0;
    exit(status);
}

//Estructura para poder asociar el nombre de un comando (ej: exit) con su código respectivo (función).
typedef struct {
    char *nombre;
    void (*funcion)(char **args); // Puntero a función
} ComandoInterno;

//Arreglo para poder buscar comandos internos (se evita recurrir a muchos condicionales).
static ComandoInterno tabla_comandos[] = {
    {"cd", ejecutar_cd},
    {"exit", ejecutar_exit},

    // DESCOMENTAR UNA VEZ IMPLEMENTADOS:
    // {"jobs", ejecutar_jobs},
    // {"pmon", ejecutar_pmon},

    {NULL, NULL} // Centinela que indica el fin del arreglo
};