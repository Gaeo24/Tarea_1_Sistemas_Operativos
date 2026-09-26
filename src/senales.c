#include <senales.h>

int configurar_senales_shell(void){
    struct sigaction shell = {0};

    //ignora señales
    shell.sa_handler = SIG_IGN;
    //no bloquea señales adicionales mientras se ejecuta el manejador
    sigemptyset(&shell.sa_mask);
    //reinicia llamadas a sistema interrumpidas
    shell.sa_flags = SA_RESTART;

    //configura (ctrl+c)
    if(sigaction(SIGINT, &shell, NULL) == -1){
        perror("SIGINT");
        return -1;
    }

    //configura (ctrl+\)
    if(sigaction(SIGQUIT, &shell, NULL) == -1){
        perror("SIGQUIT");
        return -1;
    }
    
    return 0;
}

int configurar_senales_hijo(void){
    struct sigaction hijo = {0};

    //restaura comportamiento de señales
    hijo.sa_handler = SIG_DFL;
    //no bloquea señales adicionales mientras se ejecuta el manejador
    sigemptyset(&hijo.sa_mask);
    //no activa opciones especiales
    hijo.sa_flags = 0;

    //restaura (ctrl+c)
    if(sigaction(SIGINT, &hijo, NULL) == -1){
        perror("SIGINT");
        return -1;
    }

    //restaura (ctrl+\)
    if(sigaction(SIGQUIT, &hijo, NULL) == -1){
        perror("SIGQUIT");
        return -1;
    }
    
    return 0;
}