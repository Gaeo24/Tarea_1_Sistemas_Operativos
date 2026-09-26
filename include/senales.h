#ifndef SENALES
    #define SENALES

    #include <signal.h>
    #include <unistd.h>
    #include <stdio.h>

    int configurar_senales_shell(void);
    int configurar_senales_hijo(void);
#endif