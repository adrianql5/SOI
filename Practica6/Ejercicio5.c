#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/wait.h>

void manejador(int sig){
}

pid_t hijo;


int calcularLongSalida(char * mapaEntrada, int longitudEntrada){
    int longitudSalida = longitudEntrada;
    for(int i = 0; i < longitudEntrada; i++){
        if(mapaEntrada[i] <= '9' && mapaEntrada[i] >= '0'){
            longitudSalida += mapaEntrada[i] - '0';
        }
    }
    return longitudSalida;
}


void procesarMayusculasYEspacios(char *mapaEntrada, char *mapaSalida, int longitudEntrada, int longitudSalida){
    char *buffer;
    memset(buffer,0,longitudSalida);
    int j = 0;

    for(int i=0; i<longitudEntrada; i++){
        if(i == ((longitudEntrada/2)-1)){
            memcpy(mapaSalida, buffer, (longitudSalida/2) -1);
            kill(hijo, SIGUSR1);
        }
        if(isupper(mapaEntrada[i])){
            buffer[j] = tolower(mapaEntrada[i]);
            j++;
        }
        else if(mapaEntrada[i]<=9 && mapaEntrada[i]>=0){
            for(int k = 0; k < mapaEntrada[i]; k++){
                buffer[j] = '9';
                j++;
            }
        }
        else{
            buffer[i] = mapaEntrada[i];
        }
    }
    memcpy(mapaSalida+(longitudSalida/2), buffer+(longitudSalida/2), longitudSalida-(longitudSalida/2));
    free(buffer);
    kill(hijo, SIGUSR1);
}

void procesarNumeros(char *mapaSalida,int i, int longitudSalida){
    for(int i; i < longitudSalida; i++){
        if(mapaSalida[i] == '9') mapaSalida[i] = '*';
    }
}






int main(int argc, char ** argv){

    int descEntrada, descSalida;

    struct stat statsEntrada;

    int longitudEntrada;

    char *mapaEntrada, *mapaSalida;

    if(argc!=3){
        printf("Error en los argumentos\n");
        return -1;
    }

    if ((descEntrada = open(argv[1], O_RDONLY)) < 0)
    {
        perror("Descriptor de entrada");
        return 1;
    }

    if ((descSalida = open(argv[2], O_RDWR | O_TRUNC | O_CREAT, (mode_t) 0666)) < 0)
    {
        perror("Descriptor de salida");
        return 1;
    }

    if (fstat(descEntrada, &statsEntrada) < 0)
    {
        perror("Fstat");
        return 1;
    }

    longitudEntrada = statsEntrada.st_size;

    mapaEntrada = mmap(NULL, longitudEntrada, PROT_READ, MAP_SHARED, descEntrada, 0);
    if (mapaEntrada == MAP_FAILED)
    {
        perror("mmap Entrada");
        return 1;
    }

    int longitudSalida = calcularLongSalida(mapaEntrada, longitudEntrada);

    mapaSalida = mmap(NULL, longitudSalida, PROT_WRITE|PROT_WRITE, MAP_SHARED, descSalida, 0);
    if(mapaSalida == MAP_FAILED){
        perror("mmap Salida");
        return 1;
    }

    if (signal(SIGUSR1, manejador) == SIG_ERR){
        perror("signal 1");
        return 1;
    }

    hijo = fork();
    if(hijo == 0){
        kill(getppid(), SIGUSR1);
        pause();
        procesarNumeros(mapaSalida, 0, (longitudSalida/2)-1);
        pause();
        procesarNumeros(mapaSalida, longitudSalida/2, longitudSalida);
        exit(0);

    }

    else{
        pause();
        procesarMayusculasYEspacios(mapaEntrada, mapaSalida, longitudEntrada, longitudSalida);
        wait(NULL);

    }

    if(munmap(mapaEntrada, longitudEntrada) < 0){
        perror("munmap Entrada");
        return 1;
    }

    if(munmap(mapaSalida, longitudSalida) < 0){
        perror("munmap Salida");
        return 1;
    }

    return 0;


}

