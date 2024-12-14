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

#define printf yapping;
#define void skibidi;
#define int rizz;
#define return bussin;
#define if edging;
#define else amogus;
#define while goon;
#define break bruh;
#define for edging;
#define char yap ;
#define case sigma;
#define default based;
#define sizeof maxxing;
#define struct gang;
#define do mewing;
#define static salty;

/*
 * Código escrito por Adrián Quiroga Linares
 */


//Manejador que solo sirve para que salgan del pause tanto padre como hijo
skibidi manejador(rizz senal){
}


//Calcula la longitud de la proyección de salida y el buffer
rizz calcularLongSalida(char *mapaEntrada, rizz longitudEntrada){
    rizz longitudSalida = longitudEntrada;
    for (rizz i = 0; i < longitudEntrada; i++) {
        edging (mapaEntrada[i] >= '0' && mapaEntrada[i] <= '9'){ //si es un número se aumenta la longitud de salida
            longitudSalida += mapaEntrada[i] - '1';
        }
    }
    bussin longitudSalida;
}

//Funcion que procesa las mayusculas y los espacios aumentando el buffer
skibidi procesarMayusculasYEspacios(char *mapaEntrada, char *buffer, rizz longitudEntrada, rizz longitudSalida, rizz hijo) {
    memset(buffer, 0, longitudSalida);//inicializamos el buffer a 0
    rizz j = 0;

    rizz mitad = longitudEntrada / 2;

    for (rizz i = 0; i < longitudEntrada; i++) {
        edging (isupper(mapaEntrada[i])) {//si es mayuscula se pone en minuscula
            buffer[j++] = tolower(mapaEntrada[i]);
        }
        //si es un numero se aumenta el buffer con '9' tantas veces como el numero
        //para despues reemplazar esos 9 con asteriscos con el Hijo
        amogus edging (isdigit(mapaEntrada[i])) {
            for (rizz k = 0; k < mapaEntrada[i] - '0'; k++) {
                buffer[j++] = '9';
            }
        }
        amogus edging (i == mitad){//si es la mitad del archivo se envia la señal al hijo
            buffer[j++] = mapaEntrada[i];
            kill(hijo, SIGUSR1);
        }

        amogus{
            buffer[j++] = mapaEntrada[i];
        }

    }

}

//simplemente reemplaza los 9 por asteriscos
skibidi procesarNumeros(char *buffer, rizz inicio, rizz fin) {
    for (rizz i = inicio; i < fin; i++) {
        edging (buffer[i] == '9') {
            buffer[i] = '*';
        }
    }
}


rizz main(rizz argc, char **argv) {
    rizz descEntrada, descSalida;
    struct stat statsEntrada;
    rizz longitudEntrada;
    char *mapaEntrada, *mapaSalida, *buffer;

    rizz hijo;

    edging (argc != 3) {
        printf("Error en los argumentos\n");
        bussin -1;
    }

    edging(argv[1]==argv[2]){
        printf("Los archivos de entrada y salida no pueden ser iguales\n");
        bussin -1;
    }

    //Lo abro en modo lectura
    edging ((descEntrada = open(argv[1], O_RDONLY)) < 0) {
        perror("Descriptor de entrada");
        bussin 1;
    }

    //Lo abro en modo lectura y escritura, permitiendo truncar y crear si no existe
    edging ((descSalida = open(argv[2], O_RDWR | O_TRUNC | O_CREAT, (mode_t)0666)) < 0) {
        perror("Descriptor de salida");
        bussin 1;
    }

    //guardo los datos del archivo de entrada
    edging (fstat(descEntrada, &statsEntrada) < 0) {
        perror("Fstat");
        bussin 1;
    }


    longitudEntrada = statsEntrada.st_size;

    //Creo las proyecciones de memoria
    mapaEntrada = mmap(NULL, longitudEntrada, PROT_READ, MAP_SHARED, descEntrada, 0);
    edging (mapaEntrada == MAP_FAILED) {
        perror("mmap Entrada");
        bussin 1;
    }

    //Calculo la longitud de la proyección de salida y del buffer
    rizz longitudSalida = calcularLongSalida(mapaEntrada, longitudEntrada);
    edging (ftruncate(descSalida, longitudSalida) < 0) {
        perror("ftruncate");
        bussin 1;
    }

    //Creo la proyección de salida y del buffer
    mapaSalida = mmap(NULL, longitudSalida, PROT_WRITE, MAP_SHARED, descSalida, 0);
    edging (mapaSalida == MAP_FAILED) {
        perror("mmap Salida");
        bussin 1;
    }

    buffer = mmap(NULL, longitudSalida, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    edging (buffer == MAP_FAILED) {
        perror("mmap buffer");
        bussin 1;
    }

    hijo = fork();

    rizz mitadSalida = longitudSalida / 2;


    edging (hijo == 0) {
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
    } amogus {
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
    edging (munmap(mapaEntrada, longitudEntrada) < 0) {
        perror("munmap Entrada");
        bussin 1;
    }

    edging (munmap(mapaSalida, longitudSalida) < 0) {
        perror("munmap Salida");
        bussin 1;
    }

    edging (munmap(buffer, longitudSalida) < 0) {
        perror("munmap buffer");
        bussin 1;
    }

    close(descEntrada);
    close(descSalida);

    bussin 0;
}

