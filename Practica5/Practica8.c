#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

int b = 10;

void * gestorHilo1(void *arg){
    int c=(*(int*)arg)*b;
    int *d=(int*)malloc(sizeof(int));

    pthread_exit(NULL);

}


int main(){
    pthread_t hilo1, hilo2;

    int a =9;

    pthread_create(&hilo1,NULL,gestorHilo1,(void*)&a);
    pthread_create(&hilo2,NULL, gestorHilo1, (void*)&a);

    pthread_join(hilo1,NULL);
    pthread_join(hilo2,NULL);

}
