#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int variableglobal=1;


int main(int argc, char const *argv[]){

    int variablelocal=2;

    int * variabledinamica =(int*)malloc(sizeof(int));
    *variabledinamica= 3;

    pid_t pid=fork();

    if(pid<0){
        perror("Error al crear el hijo\n");
        exit(EXIT_FAILURE);
    }    

    if(pid==0){
        printf("Hola soy el proceso hijo\n");

        printf("Valor variable global: %d, Valor dir memoria: %p\n", variableglobal, &variableglobal);
        printf("Valor variable local: %d, Valor dir memoria: %p\n", variablelocal, &variablelocal);
        printf("Valor variable dinámica: %d, Valor dir memoria: %p\n", *variabledinamica, variabledinamica);

        variableglobal=10;
        variablelocal=20;
        *variabledinamica=30;

        printf("Valor variable global modificada: %d, Valor dir memoria: %p\n", variableglobal, &variableglobal);
        printf("Valor variable local modificada: %d, Valor dir memoria: %p\n", variablelocal, &variablelocal);
        printf("Valor variable dinámica modificada: %d, Valor dir memoria: %p\n", *variabledinamica, variabledinamica);

        printf("---------------------------------------------------------------");
    }

    else{
        wait(NULL);
        printf("Hola soy el proceso Padre\n");

        printf("Valor dir memoria variable global: %p\n", &variableglobal);
        printf("Valor dir memoria variable local: %p\n", &variablelocal);
        printf("Valor dir memoria variable dinamica: %p\n",  variabledinamica);


        printf("Variables despues de que el hijo las modificase:\n");
        printf("Valor variable global: %d \n", variableglobal);
        printf("Valor variable local: %d\n", variablelocal);
        printf("Valor variable dinámica: %d\n", *variabledinamica);

    }



    return 0;
}