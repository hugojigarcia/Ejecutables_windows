#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* crearCarpetaUsuario(){
    FILE *salidaComando = popen("hostname && echo %username%","r"); //Tres líneas: nombre disposito, nombre usuario y fecha-hora
    if(salidaComando == NULL) {
        printf("Error en la apertura del fichero del comando");
        return NULL;
    }

    //Nombre dispositivo
    char dispositivo[500];
    fgets(dispositivo, 500, salidaComando);
    strtok(dispositivo, "\n");
    //Nombre usuario
    char nombre_usuario[255];
    fgets(nombre_usuario, 255, salidaComando);
    strtok(nombre_usuario, " \n");
    strcat(dispositivo, " ");//Unir el nombre de dispositivo al nombre de usuario
    strcat(dispositivo,nombre_usuario);

    //Crear directorio
    strcpy(nombre_usuario, "mkdir \"");
    strcat(nombre_usuario, dispositivo);
    strcat(nombre_usuario, "\"");
    system(nombre_usuario); //mkdir "dispositivo usuario"

    fclose(salidaComando);
    return dispositivo;
}


int main(int argc, char** argv)
{
    FILE *salida;
    salida  = fopen(".\\dns.txt","a");
    if(salida == NULL)
    {
        printf("Error en la apertura del fichero de salida");
        exit(1);
    }
    char directorioDondeGuardar[500];
    strcpy(directorioDondeGuardar, crearCarpetaUsuario());
    if(directorioDondeGuardar==NULL){ //Creación de una carpeta para los datos del usuario
        printf("Error en creación de la carpeta para los datos del usuario");
        fclose(salida);
        exit(2);
    }




    /* Ejecución del comando y procesamiento de su salida */



    fclose(salida);


    return 0;
}

