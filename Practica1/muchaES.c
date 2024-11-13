#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv){
	int i=0;
	long long operaciones=0;
	clock_t start_time, end_time;
    	double elapsed_time;

    
    	start_time = clock();
	while(1){
		FILE * ficher = fopen("muchaES.txt","w");
		fprintf("muchaBasuraaa",ficher);
		operaciones++;
		fclose(ficher);

		end_time = clock();
        	elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

        
        	if (elapsed_time >= 1.0) {
            	printf("Iteraciones en 1 segundo: %llu\n", operaciones);
            		operaciones = 0;  
            		start_time = clock();  
        	}
	}
}


