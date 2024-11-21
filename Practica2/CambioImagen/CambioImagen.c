#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

void crearZombie(){
    pid_t pid=fork();
    if(pid<0){
        perror("Error al crear el zombie\n");
        exit(EXIT_FAILURE);
    }

    if(pid==0){
        printf("Soy el proceso Zombie antes del exec, mi pid es: %d\n",getpid());

        //Execl, le pasamosla ubicacion del programa a ejecutar,
        //el nombre del programa
        //el argumento que le pasamos al programa
        //null inifica el final de la entrada de argumentos del progama
        execl("/bin/echo", "echo", "Soy el proceso hijo después de exec.", NULL);

        perror("Error al cambiar la imagen.\n");
        exit(EXIT_FAILURE);
    }
}

void crearHuerfano(){
    pid_t pid=fork();
    if(pid<0){
        perror("Error al crear el zombie\n");
        exit(EXIT_FAILURE);
    }
    if(pid==0){
        printf("Soy el proceso Huérfano, mi pid es: %d\n",getpid());
        sleep(10);
        printf("Soy el proceso Huerfano, voy a morir ahora\n");
        exit(0);
    }
}

int main(int argc, char const *argv[]){

    crearZombie();
    crearHuerfano();

    sleep(1);

    printf("El proceso padre PID: %d, esperando al hijo zombie ...\n", getpid());

    int status;

    pid_t pidZombie=wait(&status);//esto hace que el padre espere a que termine el primer proceso
    //en este caso terminará el Zombie y se almacena su pid

    if(WIFEXITED(status)){//WIFEEXITED comprueba que no hubiese ningun error
        int exit_status = WEXITSTATUS(status);//devuelve el número que pusimos en el exit
        printf("El hijo %d ha terminado con estado %d.\n", pidZombie, exit_status);
    }

    printf("El proceso padre termina.\n");

    sleep(5);//duermo al padre para que el huerfano pueda existir

    return 0;
}
