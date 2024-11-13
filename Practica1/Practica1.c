#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void main(int argc, char ** argv){
	
	int *p;
	int i,j,k=0,tam=20000000,N=40;

	for(i=0; i<N; i++){
		p=(int*)calloc(tam,sizeof(int));
		for(j=0;j<tam;j+=322){
			p[j]=i%39;
			k++;
		}
	
		sleep(1);
	}
	scanf("%d", &i);
	printf("%d\n",i);

}
