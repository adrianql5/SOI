#include <stdio.h>
#include <unistd.h>


void funcion1(int arg){
    int c=2;
    printf("Dir mem arg funcion1: %p\n",&arg);
    printf("Dir mem vLocal funcion1: %p\n", &c);

}


void funcion2(int arg){
    int d=9;

    printf("Dir mem arg funcion2: %p\n",&arg);
    printf("Dir mem vLocal funcion2: %p\n", &d);

}


int main(){
    int a,b;
    printf("Pid: %d\n", getppid());
    a=1;
    b=8;
    funcion1(a);
    funcion2(b);


    scanf("%d",&a);

    return 0;

}



