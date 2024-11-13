#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

/** Código escrito por:
 * Adrián Quiroga Linares
 * Manuel Pérez Pazos
 */

//Manejados de SIGINT
void manejador(int s){
    char b;
    int a;
    a++;
    printf("  hola\n");//Escribimos la cadena

    //En caso de superar el número establecido preguntamos si desea volver al funcionamiento normal
    if (a>2){
        printf("Deseas volver al funcionamiento original?(S/N)\n");
        scanf("%c",&b);
    }

    //Si lo desea devolvemos  SIGINT su comportamiento oritignal
    if(b == 'S'){
        if (signal(SIGINT,SIG_DFL)==SIG_ERR){
            exit(1);
        }
    }
}

void  main(){
    //cada vez que se reciba un SIGINT se ejecuta el manejador
    if (signal(SIGINT,manejador)==SIG_ERR){
    exit(1);
    }
    for(;;){

    }
}

