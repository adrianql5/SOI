#include <stdio.h>
#include <stdlib.h>

/*
 * Código escrito por:
 *
 * Izan Monterroso
 *
 * Adrián Quiroga
 *
 * Esperamos que las variables locales se almacenen en la pila.
 *
 * Las variables globales se almacenen en el segmento de datos
 * porque han sido inicializadas (caso contrario irían al BSS,
 * donde los datos aún no han sido inicializados).
 *
 * La direccion de la funcion main esperamos que se guarde en
 * el segmento de texto(donde está el código).
 *
 * La variable sin inicializar también se guarda en la pila,
 * porque  a diferencia de las globales, no se iniciala a 0 por defecto,
 * por lo que su contenido es indefinido
 */


float var1=16.5;
int var2=8954;
double var3=18.95;
double array[5]={2.3,5.6,8.7,9.9,10.4};

int main (int argc,char *argv[]){

    int varlocal1=80;
    char varlocal2='f';
    int arraylocal [2000];
    int x;

    printf("Dirección de var1: %p\n",&var1);
    printf("Direccion de var2: %p\n",&var2);
    printf("Direccion de var3: %p\n",&var3);
    printf("Direccion del array: %p\n",&array);

    printf("Direccion de varlocal1: %p\n",&varlocal1);
    printf("Direccion de varlocal2: %p\n",&varlocal2);
    printf("Direccion de arraylocal: %p\n",&arraylocal);
    printf("Variable no inicializada: %p\n",&x);

    void (*ptr_func)(int)=main;

    printf("Direccion de la función main: %p\n",ptr_func);

    scanf("%d",&x);

    return 0;
}
