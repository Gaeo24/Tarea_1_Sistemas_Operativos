#include <pmon.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>

// Banderas globales exclusivas para pmon
volatile sig_atomic_t running_pmon = 1;
volatile sig_atomic_t pmon_imprimir = 1;

// Manejador que se activa cada vez que la alarma suena (pmon).
static void manejador_pmon_alarma(int sig) {
    (void)sig;
    pmon_imprimir = 1; //Avisamos que hay que redibujar la tabla.
}

// Manejador que se activa al presionar Ctrl+C
static void manejador_pmon_salir(int sig) {
    (void)sig;
    running_pmon = 0; //Avisamos que hay que terminar el ciclo.
}


//Lee los archivos de /proc y extrae estado, memoria y tiempo de CPU en segundos
static int leer_info_proc(pid_t pid, char *estado_salida, long *rss_kb, double *cpu_segundos) {
    char ruta[256];
    char linea[1024];

    //Lee memoria residente aproximada.
    snprintf(ruta, sizeof(ruta), "/proc/%d/status", pid);
    FILE *f_status = fopen(ruta, "r");
    *rss_kb = 0; // Valor por defecto por si falla.
    if (f_status) {
        while (fgets(linea, sizeof(linea), f_status)) {
            // Buscamos la línea que empieza con "VmRSS:"
            if (strncmp(linea, "VmRSS:", 6) == 0) {
                sscanf(linea, "VmRSS: %ld", rss_kb);
                break;
            }
        }
        fclose(f_status);
    }

    //Lee el estado del proceso y tiempos de CPU
    snprintf(ruta, sizeof(ruta), "/proc/%d/stat", pid);
    FILE *f_stat = fopen(ruta, "r");
    *estado_salida = '?';
    *cpu_segundos = 0.0;
    
    if (f_stat) {
        if (fgets(linea, sizeof(linea), f_stat)) {
            // El formato de stat tiene el nombre del proceso entre paréntesis.
            // Para evitar problemas si el nombre tiene espacios, saltamos hasta el último ')'.
            char *p = strrchr(linea, ')');
            if (p) {
                p += 2; // Avanzamos al carácter del estado
                
                char estado_crudo;
                unsigned long utime = 0, stime = 0;
                
                // Leemos el estado (campo 3) y saltamos los campos 4 al 13, 
                // para leer utime (14) y stime (15)[cite: 3].
                sscanf(p, "%c %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %lu %lu", 
                       &estado_crudo, &utime, &stime);
                
                *estado_salida = estado_crudo;
                
                // Convertir Ticks a Segundos reales
                long ticks_por_segundo = sysconf(_SC_CLK_TCK);
                *cpu_segundos = (double)(utime + stime) / ticks_por_segundo;
            }
        }
        fclose(f_stat);
        return 1; // Éxito
    }
    
    return 0; // Error: el proceso probablemente murió...
}

//Función para traducir estado.
static const char* traducir_estado(char e) {
    switch(e) {
        case 'R': return "ejecutando";
        case 'S': return "durmiendo";
        case 'Z': return "zombie";
        case 'T': return "detenido";
        default:  return "desconocido";
    }
}

//Comando pmon
static void ejecutar_pmon(char **args, ShellState *shellState) {
    //Si no se entregan segundos, por defecto 2.
    int segundos = 2;
    if (args[1] != NULL) {
        segundos = atoi(args[1]);
        if (segundos <= 0) segundos = 2;
    }

    //Sobreecribimos señales para manejarlas.
    struct sigaction sa_alrm, sa_int, sa_old_alrm, sa_old_int;

    //Configuramos la alarma.
    sa_alrm.sa_handler = manejador_pmon_alarma;
    sigemptyset(&sa_alrm.sa_mask);
    sa_alrm.sa_flags = 0;

    //Configuramos Ctrl+C para terminar.
    sa_int.sa_handler = manejador_pmon_salir;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;

    //Añadimos nueva configuración y guardamos las originales.
    sigaction(SIGALRM, &sa_alrm, &sa_old_alrm);
    sigaction(SIGINT, &sa_int, &sa_old_int);

    //---Ciclo del comando---

    running_pmon = 1;
    pmon_imprimir = 1;

    //Para guardar el tiempo de la cpu anterior y post hacer el cálculo.
    double *cpu_previo = calloc(shellState->capacidad_jobs, sizeof(double));

    while (running_pmon) {
        if (pmon_imprimir) {
            pmon_imprimir = 0; //En 0 para que no imprima enseguida.

            printf("\nRefrescando pmon cada %d segundos... (Ctrl+C para salir)\n", segundos);
            printf("PID\t| COMANDO\t| ESTADO\t| %%CPU\t| RSS (KB)\n");

            // Bloqueamos SIGCHLD.
            sigset_t mascara, mascara_vieja;
            sigemptyset(&mascara);
            sigaddset(&mascara, SIGCHLD);
            sigprocmask(SIG_BLOCK, &mascara, &mascara_vieja);

            // Recorremos todos los procesos activos.
            for (int i = 0; i < shellState->capacidad_jobs; i++) {
                if (shellState->jobs[i].activo) {
                
                    char estado_letra;
                    long rss_kb;
                    double cpu_actual;

                    // Si la función logra leer la carpeta
                    if (leer_info_proc(shellState->jobs[i].pid, &estado_letra, &rss_kb, &cpu_actual)) {
                        
                        // Cálculo del porcentaje de CPU
                        double delta_cpu = cpu_actual - cpu_previo[i];
                        double porcentaje_cpu = (delta_cpu / segundos) * 100.0;
                        
                        // Aseguramos que en la primera lectura (delta = cpu_actual) no imprima un % gigante
                        if (cpu_previo[i] == 0.0) porcentaje_cpu = 0.0; 
                        
                        // Guardamos el actual para la próxima iteración
                        cpu_previo[i] = cpu_actual;

                        // Imprimimos la fila de la tabla
                        printf("%d\t| %s\t| %s\t| %.1f\t| %ld\n",
                            shellState->jobs[i].pid,
                            shellState->jobs[i].comando,
                            traducir_estado(estado_letra),
                            porcentaje_cpu,
                            rss_kb);
                    }
                } else {
                    // Si la ranura no está activa, limpiamos su historial de CPU por si se reutiliza
                    cpu_previo[i] = 0.0;
                }
            }

            // Desbloqueamos SIGCHLD
            sigprocmask(SIG_SETMASK, &mascara_vieja, NULL);

            //Configuramos alarma para poder actualizar según la variable segundos.
            alarm(segundos);
        }

        pause(); //Buena práctica para dormir la ejecución hasta la alarma.
    }
    
    //Liberamos memoria llamada con calloc.
    free(cpu_previo);

    //Si se sale del ciclo...
    alarm(0); //Cancelamos cualquier alarma pendiente.
    printf("\nSaliendo del monitor pmon...\n");

    //Volvemos a la configuración original de la shell.
    sigaction(SIGALRM, &sa_old_alrm, NULL);
    sigaction(SIGINT, &sa_old_int, NULL);
}