#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char ** argv){
	double d=1.324213412, e=2.2341234, resultado;
	unsigned long long iteraciones=0;
	clock_t start_time, end_time;
    	double elapsed_time;
	start_time=clock();
	while(1){
		resultado= (d+e)/d+e;
		iteraciones++;
		end_time = clock();
       		elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

		if (elapsed_time >= 1.0) {
            		printf("Iteraciones en 1 segundo: %llu\n", iteraciones);
            		iteraciones = 0;  
            		start_time = clock();  
        	}
	}
}
	
