#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define FILENAME "partido.txt"


/**
 * Código escrito por:
 * Adrián Quiroga Linares
 * Manuel Pérez Pazos
 */



pid_t maquina, jugador;
int fd; //descriptor de archivo donde se guardan los datos

//Variables sobre el juego
int posicionJugador = 0;
int posicionMaquina = 0;
int posicionBola = 0;
int puntosJugador = 0;
int puntosMaquina = 0;

//Variables sobre el control del juego
/**
 * Lanzar Mover Suceso
 *   0     0    Jugador acaba de sacar
 *   0     1    Jugador acaba de Mover
 *   1     0    Maquina acaba de sacar
 *   1     1    Maquina acaba de Mover
 */

int Lanzar = 0;
int Mover = 0;
int puntoAcabado = 0; //1 -> el punto a acabado , 0-> el punto sigue en juego

//Funcion que escribe el estado del juego en el archivo
void escribirEstado() {
    lseek(fd, 0, SEEK_SET); //Ponem0s el puntero de archivo al inicio de todo
    dprintf(fd, "%d %d %d %d %d %d %d %d\n", puntosMaquina, puntosJugador,
          posicionMaquina, posicionJugador, posicionBola, puntoAcabado, Lanzar,
          Mover); //Escribimos los datos en el archivo
    fsync(fd); //Aseguramos la escritura de los datos en el archivo
}

//Funcion que leer los datos del archivo
void leerEstado() {
    lseek(fd, 0, SEEK_SET); //Movemos el puntero de archivo al inicio
    fscanf(fdopen(fd, "r"), "%d %d %d %d %d %d %d %d", &puntosMaquina,
         &puntosJugador, &posicionMaquina, &posicionJugador, &posicionBola,
         &puntoAcabado, &Lanzar, &Mover);//almacenamos los datos
}

//Gestor de las acciones del jugador
static void gestorJugador(int señal) {
    leerEstado();//leemos los datos
    int nuevaPosicion;
    switch (señal) {
        case SIGUSR1:
            //En este caso el jugador se tiene que mover
            printf("Introduce la posición del jugador (0-9): M[%d] J[%d]\n", posicionMaquina, posicionJugador);
            scanf("%d", &nuevaPosicion);

            //Comprobamos la validez del movimiento
            if (abs(nuevaPosicion - posicionJugador) > 2 || nuevaPosicion < 0 || nuevaPosicion > 9) {
                printf("Movimiento no válido. El jugador se queda en la posición %d.\n", posicionJugador);
            }
            else {
                posicionJugador = nuevaPosicion;
            }

            //Actualizamos nuestras variables de control para saber que acaba de moverse el jugador
            Lanzar = 0;
            Mover = 1;
            escribirEstado();//escribimos el estado
            kill(getppid(), SIGUSR1);//llamamos al padre para que continúe el juego
            break;

        case SIGUSR2:
            //En este caso el jugador saca
            printf("El jugador toca la bola ¿A qué posición quiere enviar la bola? (0-9): M[%d] J[%d]\n", posicionMaquina, posicionJugador);
            scanf("%d", &nuevaPosicion);

            //Verificamos la validez del moviemiento
            if (nuevaPosicion < 0 || nuevaPosicion > 9) {
                printf("Posición no válida. La bola se envía a la posición 0.\n");
                posicionBola = 0;
            }
            else {
                posicionBola = nuevaPosicion;
            }

            //Actualizamos las variables de control, acaba de sacar el jugador
            Lanzar = 0;
            Mover = 0;
            escribirEstado();
            kill(getppid(), SIGUSR1);
            break;
    }
}

//Gestor de las acciones de la maquina
static void gestorMaquina(int señal) {
    leerEstado();
    int nuevaPosicion;
    switch (señal) {
        case SIGUSR1:
            //Asignamos una posicion aleatoria que cumpla con el requisito de que no se puede mover mas de 2 posiciones desde la anterior
            do {
            nuevaPosicion = rand() % 10;
            } while (abs(nuevaPosicion - posicionMaquina) > 2);
            posicionMaquina = nuevaPosicion;

            //Actualizamos las variables de control, la máquina acaba de moverse
            Lanzar = 1;
            Mover = 1;
            printf("La maquina se mueve a a la posicion %d\n", posicionMaquina);
            escribirEstado();
            kill(getppid(), SIGUSR1);
            break;

        case SIGUSR2:
            //Asignamos una posicion aleatoria a la maquina
            posicionBola = rand() % 10;

            //Actualizamos las variables de control, la maquina acaba de lanzar la bola
            Lanzar = 1;
            Mover = 0;
            printf("La maquina toca la bola y la manda a la posicion %d\n",posicionBola);
            escribirEstado();
            kill(getppid(), SIGUSR1);
            break;
    }
}

//Gestor de las acciones del Padre
static void gestorPadre(int señal){
    //no hace nada, solo recibe señales enviadas por sus hijos para que salga de los pauses del main y poder continuar con la partida
}

//Funcion que asigna las posiciones iniciales
void asignarPosicionesIniciales() {
    printf("Introduce la posición inicial del jugador: \n");
    scanf("%d", &posicionJugador);
    if (posicionJugador < 0 || posicionJugador > 9) {
        printf("Posición no válida. Se asigna la posición 0 por defecto.\n");
        posicionJugador = 0;
    }
    posicionMaquina = rand() % 10;
    printf("La posición inicial de la máquina es: %d\n", posicionMaquina);
    escribirEstado();
}

//Funcion que evalua si el punto acaba o continua
int evaluarJugada() {
    leerEstado();//Leemos el estado del partido

    //Si la maquina se acaba de mover evaluamos si llega o no llega a la pelota
    if (Lanzar && Mover) {
        if (abs(posicionMaquina - posicionBola) > 2) {
        puntosJugador++;
        escribirEstado();
        printf("La maquina no llega. El jugador gana el punto\n");
        return 1;//Si no llega rompe el bucle
        }
    }

    //Si el jugador se acaba de mover evaluamos si llega o no llega a la pelota
    else if (!Lanzar && Mover) {
        if (abs(posicionJugador - posicionBola) > 2) {
        puntosMaquina++;
        escribirEstado();
        printf("El jugador no llega. La máquina gana el punto\n");
        return 1;//Si no llega rompe el bucle
        }
    }
    return 0;//Si llega no pasa nada, continua el punto
}

//Funcion que en base a la accion anterior evalua cual sera la siguiente accion
void evaluarAccion() {
    leerEstado();//Leemos el estado del partido

    //Si el jugador acaba de lanzar
    if (!Lanzar && !Mover) {
        kill(maquina, SIGUSR1);//la maquina se debe mover
    }

    //Si el jugador acaba de moverse
    else if (!Lanzar && Mover) {
        kill(jugador, SIGUSR2);//el jugador debe sacar
    }

    //Si la maquina acaba de lanzar
    else if (Lanzar && !Mover) {
        kill(jugador, SIGUSR1);//El jugador debe moverse
    }

    //Si la maquina acaba de moverse
    else if (Lanzar && Mover) {
        kill(maquina, SIGUSR2);//La maquina debe lanzar la bola
    }
}

int main() {
    srand(time(NULL)); //inciamos el generador de números aleatorios
    fd = open(FILENAME, O_RDWR | O_CREAT | O_TRUNC, 0666); //abrimos el archivo para escribir en el los datos
    if (fd < 0) {
        perror("Error al abrir archivo");
        exit(EXIT_FAILURE);
    }

    escribirEstado();

    //Creamos el hijo jugador
    jugador = fork();
    if (jugador == 0) {
        //le asignamos un manejados a las señales SIGUSR1 y SIGUSR2
        signal(SIGUSR1, gestorJugador);
        signal(SIGUSR2, gestorJugador);
        while (1)
          pause();
        return 0;
    }

    //Creamos el hijo maquina
    maquina = fork();
    if (maquina == 0) {
        signal(SIGUSR1, gestorMaquina);
        signal(SIGUSR2, gestorMaquina);
        while (1)
          pause();
        return 0;
    }


    //cuando el padre detecte un SIGUSR1 simplemete saldrá de los pauses
    signal(SIGUSR1, gestorPadre);

    //Asignamos las posiciones iniciales
    asignarPosicionesIniciales();
    int sacaJugador = 0;

    //mientras que uno de los jugadores no supere la maxima puntuacion
    while (puntosJugador < 10 && puntosMaquina < 10) {
        sacaJugador = rand() % 2;//decimos aletoriamente quien saca
        leerEstado();
        printf("MARCADOR: Maquina [%d] Jugador [%d]\n", puntosMaquina, puntosJugador);
        printf("POSICIONES: Maquina [%d] Jugador [%d]\n", posicionMaquina,posicionJugador);
        printf("Saca %s\n", sacaJugador ? "el Jugador" : "la Maquina");

        //Si saca el jugador
        if (sacaJugador) {
          kill(jugador, SIGUSR2);//el jugador saca la bola y espera.
          pause();//El padre espera a recibir una señal por parte del hijo jugador
          do {
            evaluarAccion();//Evaluamos la siguiente accion mientras que no acabe el punto
            pause();
          } while (!evaluarJugada());
        } else {
          kill(maquina, SIGUSR2);
          pause();
          do {
            evaluarAccion();
            pause();
          } while (!evaluarJugada());
        }
        leerEstado();
    }

    //Si acaba el partido matamos a los hijos.
    printf("Juego terminado. Puntuación final: Jugador %d - Máquina %d\n",puntosJugador, puntosMaquina);
    kill(maquina, SIGTERM);
    kill(jugador, SIGTERM);
    close(fd);
    exit(EXIT_SUCCESS);
    return 0;
}
