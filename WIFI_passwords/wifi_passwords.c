#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int wifi_passwords(int argc, char** argv)
{
    FILE *salida;
    salida  = fopen(".\\dns.txt","a");
    if(salida == NULL)
    {
        printf("Error en la apertura del fichero de salida");
        exit(1);
    }

    /* Ejecución del comando y procesamiento de su salida */
    FILE *entrada = popen("ipconfig/displaydns","r");
    if(entrada == NULL) {
        printf("Error en la apertura del fichero del comando");
        fclose(salida);
        exit(2);
    }
    /*char linea[500];
    char lineaAnterior[500];

    while(fgets(linea, 500, entrada))
    {
        drop_n_left(linea, 4);
        if(linea[0]=='-')
        {
            printf("%s", lineaAnterior);
            if(argc==2) fputs(lineaAnterior, salida);
        }
        strcpy(lineaAnterior, linea);
    }*/

    fclose(entrada);
    fclose(salida);

    return 0;
}

