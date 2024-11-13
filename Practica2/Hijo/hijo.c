#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char const *argv[]){
    pid_t pid =fork();
    if(pid<0){
        perror("Error al crear el hijo.\n");
        exit(EXIT_FAILURE);
    }    

    if(pid==0){
        printf("Soy el proceso hijo.\n");
        printf("PID: %d\n", getpid());
        printf("PID de mi padre: %d\n", getppid());
        printf("UID real: %d\n", getuid());
        printf("UID efectivo: %d\n",geteuid());
        printf("GID: %d \n",getgid());
        printf("Variable de entorno Path: %s\n", getenv("PATH"));
        printf("------------------------\n");
        exit(EXIT_SUCCESS);
    }

    else{
        wait(NULL);
        printf("Soy el proceso padre.\n");
        printf("PID: %d\n", getpid());
        printf("PID de mi padre: %d\n", getppid());
        printf("UID real: %d\n", getuid());
        printf("UID efectivo: %d\n",geteuid());
        printf("GID: %d \n",getgid());
        printf("Variable de entorno Home: %s\n", getenv("HOME"));
        printf("------------------------\n");
    }


    return 0;
}
