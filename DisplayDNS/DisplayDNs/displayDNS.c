#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void drop_n_left(char *s, int n)
{
    if(strlen(s)>1)
    {
        char* s2 = s + n;
        while ( *s2 )
        {
            *s = *s2;
            ++s;
            ++s2;
        }
        *s = '\0';
    }
}


int main(int argc, char** argv)
{
    FILE *salida;
    /*Formato entrada:
        -DisplayDNS
        -Opcional: salida en fichero (w, a)
        -Siempre por defecto null
    */

    /* Comprobación de parámetros y apertura del fichero*/
    if(argc>2)
    {
        printf("Error en el paso de parámetros");
        exit(1);
    }
    else if(argc==2) /* Si hay parámetro de apertura de fichero */
    {
        if(strcmp(argv[1], "w")==0 || strcmp(argv[1], "a")==0 ) /* Si el parámetro de apertura de fichero es correcto */
        {
            salida  = fopen(".\\dns.txt",argv[1]);
            if(salida == NULL)
            {
                printf("Error en la apertura del fichero de salida");
                exit(3);
            }


        } else {
            printf("Error en el paso del parametro de apertura de fichero");
            exit(2);
        }

    }

    /* Ejecución del comando y procesamiento de su salida */
    FILE *entrada = popen("ipconfig/displaydns","r");
    char linea[500];
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
    }

    fclose(entrada);
    if(argc==2) {
        fclose(salida);
    }

    return 0;
}

