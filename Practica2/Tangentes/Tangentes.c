#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>

#define numerop 70000000
#define nombre "Tangentes.txt"


double max(double a, double b){
    if(a>=b){
        return a;
    }
    return b;
}


// Función que imprime en el txt la información de cada proceso en función de si calculamos Todos, solo Impares o solo Pares
void calcularTangente(int n, int incremento, char *str, FILE *archivo) {
    double suma = 0.0;
    int i;
    double media=0.0;
    struct timeval inicio, fin;
    gettimeofday(&inicio, NULL); // comenzamos a medir el tiempo

    for (i = n; i <= numerop; i += incremento) {
        suma += tan(sqrt(i)); // realizamos la suma
    }

    media = suma / numerop; // calculamos la media
    gettimeofday(&fin, NULL);

    double tiempo = (fin.tv_sec - inicio.tv_sec )+ (fin.tv_usec - inicio.tv_usec) / 1000000.0; // calculamos el tiempo

    if (archivo != NULL) {
        // printeamos los datos en el txt
        if(strcmp(str,"Total")==0)fprintf(archivo, "Proceso %s: Media = %.15lf Tiempo = %.15lf segundos Pid: %d\n", str, media, tiempo, getpid());
        else fprintf(archivo, "Proceso %s: Media = %.15lf Tiempo = %.15lf segundos Pid: %d\n", str, 2 * media, tiempo, getpid());//imprime el resultado sobre 35 millones
    } else {
        perror("Error al escribir en el archivo");
    }

    // printeamos datos de control por la pantalla
    if(strcmp(str,"Total")==0)printf("Proceso %s Pid: %d Media: %.15lf \n", str, getpid(), media);
    else printf("Proceso %s Pid: %d Media: %.15lf \n", str, getpid(), 2 * media);
    exit(0);
}

int main() {
    int pidProc1, pidProc2, pidProc3, pidProc4;

    // El proceso padre abre el archivo
    FILE *archivo = fopen(nombre, "w+");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    pidProc1 = fork();
    if (pidProc1 < 0) {
        perror("Error al crear el primer hijo");
        exit(EXIT_FAILURE);
    }
    // Primer hijo, calcula la suma de los nº pares
    if (pidProc1 == 0) {
        calcularTangente(2, 2, "Pares", archivo);
    }

    pidProc2 = fork();
    if (pidProc2 < 0) {
        perror("Error al crear el segundo hijo");
        exit(EXIT_FAILURE);
    }

    // Segundo hijo, calcula la suma de los nº impares
    if (pidProc2 == 0) {
        calcularTangente(1, 2, "Impares", archivo);
    }

    // Tercer hijo, calcula la suma de todos los nº
    pidProc3 = fork();
    if (pidProc3 < 0) {
        perror("Error al crear el tercer hijo");
        exit(EXIT_FAILURE);
    }
    if (pidProc3 == 0) {
        calcularTangente(1, 1, "Total", archivo);
    }

    // Esperamos a que acaben los procesos 1 y 2 para poder usarlos en el proceso 4
    waitpid(pidProc1, NULL, 0);
    waitpid(pidProc2, NULL, 0);

    pidProc4 = fork();
    if (pidProc4 < 0) {
        perror("Error al crear el cuarto hijo");
        exit(EXIT_FAILURE);
    }
    if (pidProc4 == 0) {
        char buffer[256];
        double mediaPares = 0.0, mediaImpares = 0.0;
        double tiempoPares = 0.0, tiempoImpares = 0.0;

        // Leemos línea a línea el archivo
        rewind(archivo); // nos aseguramos de leer desde el inicio del archivo
        while (fgets(buffer, sizeof(buffer), archivo)) {
            if (strncmp(buffer, "Proceso Impares:", 16) == 0) {
                // Escaneamos los datos que nos interesan
                sscanf(buffer, "Proceso Impares: Media = %lf Tiempo = %lf", &mediaImpares, &tiempoImpares);
            } else if (strncmp(buffer, "Proceso Pares:", 14) == 0) {
                sscanf(buffer, "Proceso Pares: Media = %lf Tiempo = %lf", &mediaPares, &tiempoPares);
            }
        }

        double mediacomb = (mediaImpares + mediaPares)/2;
        double tiempocomb = max(tiempoPares, tiempoImpares); //como se ejecutan en paralelo cogemos el que más tarda

        printf("Proceso 4 Media = %lf Tiempo Proceso 4 = %lf Pid = %d\n", mediacomb, tiempocomb, getpid());

        // Printeamos la información del proceso 4
        fprintf(archivo, "Proceso 4: Media = %.15lf Tiempo = %.15lf segundos Pid: %d\n", mediacomb, tiempocomb, getpid());

        exit(0);
    }

    // Esperamos a que terminen ambos procesos
    waitpid(pidProc3, NULL, 0);
    waitpid(pidProc4, NULL, 0);

    // Proceso padre analiza los resultados de los procesos 3 y 4
    char buffer[256];
    double media3 = 0, media4 = 0;
    double tiempo3 = 0, tiempo4 = 0;

    rewind(archivo); // Nos aseguramos de leer desde el inicio del archivo
    // Leemos línea a línea y almacenamos los datos
    while (fgets(buffer, sizeof(buffer), archivo)) {
        if (strncmp(buffer, "Proceso Total:", 14) == 0) {
            sscanf(buffer, "Proceso Total: Media = %lf Tiempo = %lf", &media3, &tiempo3);
        } else if (strncmp(buffer, "Proceso 4:", 10) == 0) {
            sscanf(buffer, "Proceso 4: Media = %lf Tiempo = %lf", &media4, &tiempo4);
        }
    }

    // Printeamos el análisis de resultados
    fprintf(archivo, "\nAnálisis resultados:\n");
    fprintf(archivo, "Media hijo 3: %.15lf\n", media3);
    fprintf(archivo, "Media hijo 4: %.15lf\n", media4);
    fprintf(archivo, "Diferencia absoluta precisión entre hijo 3 y hijo 4: %.15lf\n", fabs(media3 - media4));
    fprintf(archivo, "Tiempo hijo 3: %.15lf segundos \nTiempo hijo 4: %.15lf segundos\n", tiempo3, tiempo4);
    fprintf(archivo,"Diferencia de tiempo entre hijo 3 e hijo 4: %.15lf\n",fabs(tiempo3-tiempo4));
    printf("Diferencia de precisión entre los hijos 3 y 4: %.15lf\n", fabs(media3 - media4));
    printf("Diferencia de tiempo entre hijo 3 e hijo 4: %.15lf\n", fabs(tiempo3-tiempo4));

    // Cerramos el archivo una vez
    fclose(archivo);

    return 0;
}
