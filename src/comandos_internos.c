#include <signal.h>
#include <comandos_internos.h>
#include <shell.h>

/* Comando cd */
static void ejecutar_cd(char **args, ShellState *shellState) {
    (void)shellState;/*Aquí no se usa, pero declararlo evita warnings.*/
    
    /*Se validan exceso de argumentos.*/
    if (args[1] != NULL && args[2] != NULL) {
        fprintf(stderr, "miShell: cd: demasiados argumentos\n");
        return; 
    }
    
    /*Verifica argumento luego de cd */
    char *dir = args[1] != NULL ? args[1] : getenv("HOME");
    /*Si no está definido "HOME" lanza error.*/
    if (dir == NULL) fprintf(stderr, "miShell: cd: HOME no definido\n");
    /*Se intenta cambiar de directorio, si no es posible, lanza error.*/
    else if (chdir(dir) != 0) perror("miShell: cd");
}

/*Comando exit*/
static void ejecutar_exit(char **args, ShellState *shellState) {
    /*Antes de cerrar la shell, se guarda estado si el usuario lo ingresó. Por defecto 0.*/
    shellState->exit_status = args[1] != NULL ? atoi(args[1]) : 0;
    shellState->running = 0; /*El ciclo terminará limpiamente.*/
}

/*Comando jobs*/
static void ejecutar_jobs(char **args, ShellState *shellState) {
    (void)args;

    /*Creamos una máscara y añadimos SIGCHLD*/
    sigset_t mascara_bloqueo, mascara_original;
    sigemptyset(&mascara_bloqueo);
    sigaddset(&mascara_bloqueo, SIGCHLD);

    /*Aplicamos la configuración (bloqueamos) y conservamos la original.*/
    sigprocmask(SIG_BLOCK, &mascara_bloqueo, &mascara_original);

    /*Imprimimos los jobs*/
    for (int i = 0; i < shellState->capacidad_jobs; i++) {
        /*Solo imprimimos si la ranura está activa*/
        if (shellState->jobs[i].activo) {
            /*Imprime en el formato pedido: [1] Ejecutando sleep 30*/
            printf("[%d]\tPID: %d\tEstado: Ejecutando\t%s\n", 
                   shellState->jobs[i].id, 
                   shellState->jobs[i].pid, 
                   shellState->jobs[i].comando);
        }
    }

    /*Desbloqueamos las señales*/
    sigprocmask(SIG_SETMASK, &mascara_original, NULL);
}

/*Estructura para poder asociar el nombre de un comando (ej: exit) con su código respectivo (función).*/
typedef struct {
    char *nombre;
    void (*funcion)(char **args, ShellState *shellState); /* Puntero a función */
} ComandoInterno;

/*Arreglo para poder buscar comandos internos (se evita recurrir a muchos condicionales).*/
static ComandoInterno tabla_comandos[] = {
    {"cd", ejecutar_cd},
    {"exit", ejecutar_exit},
    {"jobs", ejecutar_jobs},
    /* {"pmon", ejecutar_pmon},*/

    {NULL, NULL} /* Centinela que indica el fin del arreglo*/
};

/*Comparamos el string ingresado con los comandos internos en el arreglo.*/
int ejecutar_comando_internos(char **tokens, ShellState *shellState) {
    if (tokens[0] == NULL) return 0;

    /*Recorremos el arreglo comparando*/
    for (int i = 0; tabla_comandos[i].nombre != NULL; i++) {
        if (strcmp(tokens[0], tabla_comandos[i].nombre) == 0) {
            tabla_comandos[i].funcion(tokens, shellState); /* Ejecutamos el código asociado al comando (función).*/
            return 1; 
        }
    }

    return 0; /*Si el comando interno no está en la tabla...*/
}