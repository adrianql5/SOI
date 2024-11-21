#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>


int main(){

    pid_t hijo;
    hijo=fork();
    if(hijo==0){
        char *a;
        a=(char*) malloc(500*sizeof(char));
        char* arr[] = { NULL};
        execv("/home/adrianql/Escritorio/Practica5/Practica5.c",arr);

        scanf("%s",a);
    }
    waitpid(hijo,NULL,0);


    return 0;
}



