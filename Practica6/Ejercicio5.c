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


/*
 * Código escrito por Adrián Quiroga Linares
 */


//Manejador que solo sirve para que salgan del pause tanto padre como hijo
void manejador(int senal){
}


//Calcula la longitud de la proyección de salida y el buffer
int calcularLongSalida(char *mapaEntrada, int longitudEntrada){
    int longitudSalida = longitudEntrada;
    for (int i = 0; i < longitudEntrada; i++) {
        if (mapaEntrada[i] >= '0' && mapaEntrada[i] <= '9'){ //si es un número se aumenta la longitud de salida
            longitudSalida += mapaEntrada[i] - '1';
        }
    }
    return longitudSalida;
}

//Funcion que procesa las mayusculas y los espacios aumentando el buffer
void procesarMayusculasYEspacios(char *mapaEntrada, char *buffer, int longitudEntrada, int longitudSalida, int hijo) {
    memset(buffer, 0, longitudSalida);//inicializamos el buffer a 0
    int j = 0;

    int mitad = longitudEntrada / 2;

    for (int i = 0; i < longitudEntrada; i++) {
        if (isupper(mapaEntrada[i])) {//si es mayuscula se pone en minuscula
            buffer[j++] = tolower(mapaEntrada[i]);
        }
        //si es un numero se aumenta el buffer con '9' tantas veces como el numero
        //para despues reemplazar esos 9 con asteriscos con el Hijo
        else if (isdigit(mapaEntrada[i])) {
            for (int k = 0; k < mapaEntrada[i] - '0'; k++) {
                buffer[j++] = '9';
            }
        }
        else if (i == mitad){//si es la mitad del archivo se envia la señal al hijo
            buffer[j++] = mapaEntrada[i];
            kill(hijo, SIGUSR1);
        }

        else{
            buffer[j++] = mapaEntrada[i];
        }

    }

}

//simplemente reemplaza los 9 por asteriscos
void procesarNumeros(char *buffer, int inicio, int fin) {
    for (int i = inicio; i < fin; i++) {
        if (buffer[i] == '9') {
            buffer[i] = '*';
        }
    }
}


int main(int argc, char **argv) {
    int descEntrada, descSalida;
    struct stat statsEntrada;
    int longitudEntrada;
    char *mapaEntrada, *mapaSalida, *buffer;

    int hijo;

    if (argc != 3) {
        printf("Error en los argumentos\n");
        return -1;
    }

    if(argv[1]==argv[2]){
        printf("Los archivos de entrada y salida no pueden ser iguales\n");
        return -1;
    }

    //Lo abro en modo lectura
    if ((descEntrada = open(argv[1], O_RDONLY)) < 0) {
        perror("Descriptor de entrada");
        return 1;
    }

    //Lo abro en modo lectura y escritura, permitiendo truncar y crear si no existe
    if ((descSalida = open(argv[2], O_RDWR | O_TRUNC | O_CREAT, (mode_t)0666)) < 0) {
        perror("Descriptor de salida");
        return 1;
    }

    //guardo los datos del archivo de entrada
    if (fstat(descEntrada, &statsEntrada) < 0) {
        perror("Fstat");
        return 1;
    }


    longitudEntrada = statsEntrada.st_size;

    //Creo las proyecciones de memoria
    mapaEntrada = mmap(NULL, longitudEntrada, PROT_READ, MAP_SHARED, descEntrada, 0);
    if (mapaEntrada == MAP_FAILED) {
        perror("mmap Entrada");
        return 1;
    }

    //Calculo la longitud de la proyección de salida y del buffer
    int longitudSalida = calcularLongSalida(mapaEntrada, longitudEntrada);
    if (ftruncate(descSalida, longitudSalida) < 0) {
        perror("ftruncate");
        return 1;
    }

    //Creo la proyección de salida y del buffer
    mapaSalida = mmap(NULL, longitudSalida, PROT_WRITE, MAP_SHARED, descSalida, 0);
    if (mapaSalida == MAP_FAILED) {
        perror("mmap Salida");
        return 1;
    }

    buffer = mmap(NULL, longitudSalida, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (buffer == MAP_FAILED) {
        perror("mmap buffer");
        return 1;
    }

    hijo = fork();

    int mitadSalida = longitudSalida / 2;


    if (hijo == 0) {
        signal(SIGUSR1, manejador);
        //aviso al padre de que el hijo esta listo
        kill(getppid(), SIGUSR2);

        //espero a que el padre haya acabado la mitad de su trabajo
        pause();
        procesarNumeros(buffer, 0, mitadSalida);

        //aviso al padre de que el hijo ha acabado
        kill(getppid(), SIGUSR2);
        pause();
        procesarNumeros(buffer, mitadSalida, longitudSalida);

        exit(0);
    } else {
        signal(SIGUSR2, manejador);
        pause();//espero a que el hijo haya sido creado correctamente
        procesarMayusculasYEspacios(mapaEntrada, buffer, longitudEntrada, longitudSalida,hijo);

        //Espero a que el hijo haya acabado la primera parte de su trabajo para mandar el SIGUSR1 y evitar problemas
        //de coordinación
        pause();
        kill(hijo, SIGUSR1);
        wait(NULL);
    }

    //Copio en el mapa de salida el buffer
    memcpy(mapaSalida, buffer, longitudSalida);

    //Desmapeo y cierro los archivos
    if (munmap(mapaEntrada, longitudEntrada) < 0) {
        perror("munmap Entrada");
        return 1;
    }

    if (munmap(mapaSalida, longitudSalida) < 0) {
        perror("munmap Salida");
        return 1;
    }

    if (munmap(buffer, longitudSalida) < 0) {
        perror("munmap buffer");
        return 1;
    }

    close(descEntrada);
    close(descSalida);

    return 0;
}

