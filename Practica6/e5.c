#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>
#include <sys/stat.h>


void manejador(int señal){

}


char* procesarMayusculasYHuecos(const char *archivo_entrada, int inicio, int final){
    int j = inicio;
    char * buffer = (char*)malloc(sizeof(final));
    for(int i = inicio; i<final; i++){
        if(isupper(archivo_entrada[i])){
            buffer[j] = archivo_entrada[i];
            j++;
        }
        else if(isdigit(archivo_entrada[i])){
            for(int k = 0; k<archivo_entrada[i]-'0'; k++){
                buffer=(char*)realloc(buffer, sizeof(1));
                buffer[j] = '9';
                j++;
            }
        }
        else{
            buffer[j]=archivo_entrada[i];
            j++;
        }
    }

    void cambiarNuevesPorAsteriscos(char *buffer){
        for(int i = 0; i<strlen(buffer); i++){
            if(buffer[i]=='9'){
                buffer[i]='*';
            }
        }
    }


    int main(int argc, char ** argv){
        if(argc!=3){
            fprintf(stderr, "Uso: %s <archivo entrada> <archivo salida>\n");
            return EXIT_FAILURE;
        }

        int fd = open(argv[1], O_RDONLY);
        if(fd==-1){
            perror("Error al abrir el archivo de entrada");
            return EXIT_FAILURE;
        }

        struct stat st;
        if(fstat(fd, &st)==-1){
            perror("Error al obtener el tamaño del archivo");
            return EXIT_FAILURE;
        }

        int longitud_archivo_entrada = st.st_size;

        char *archivo_entrada = mmap(NULL, longitud_archivo_entrada, PROT_READ, MAP_PRIVATE, fd, 0);
        if(archivo_entrada==MAP_FAILED){
            perror("Error al mapear el archivo de entrada");
            return EXIT_FAILURE;
        }

        int fd_salida = open(argv[2], O_RDWR|O_CREAT|O_TRUNC, 0666);
        if(fd_salida==-1){
            perror("Error al abrir el archivo de salida");
            return EXIT_FAILURE;
        }

        char *archivo_salida = mmap(NULL, longitud_archivo_entrada, PROT_READ|PROT_WRITE, MAP_SHARED, fd_salida, 0);
        if(archivo_salida==MAP_FAILED){
            perror("Error al mapear el archivo de salida");
            return EXIT_FAILURE;
        }

        signal(SIGUSR1, manejador);

        pid_t pid = fork();
        if(pid==-1){
            perror("Error al crear el proceso hijo");
            return EXIT_FAILURE;
        }

        size_t mitad = longitud_archivo_entrada/2;

        if(hijo==0){
            kill(getppid(), SIGUSR1);

            pause();

            cambiarNuevesPorAsteriscos(archivo_salida);

            kill(getppid(), SIGUSR1);
            cambiarNuevesPorAsteriscos(archivo_salida);

            exit(EXIT_SUCCESS);
        }
        else{
            pause();
            char *buffer = procesarMayusculasYHuecos(archivo_entrada, 0, mitad);

            ftruncate(fd_salida, strlen(buffer));
            int tam1= strlen(buffer);

            write(fd_salida, buffer, strlen(buffer));
            free(buffer);

            kill(hijo, SIGUSR1);

            pause();

            char *buffer2 = procesarMayusculasYHuecos(archivo_entrada, mitad+1, longitud_archivo_entrada);
            ftruncate(fd_salida, strlen(buffer2)+tam1);

            write(fd_salida, buffer2, strlen(buffer2));
            free(buffer2);

            wait(NULL);

        }

    }
}
