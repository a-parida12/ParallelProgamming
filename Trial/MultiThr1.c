#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

//the function for the thread
void* hello(void* args){
	printf("Hello World from Pthread\n");
	
	return NULL;
}

int main(){

	int num_threads=3;// specify the no. of threads in use

	pthread_t* thread = (pthread_t*) malloc(num_threads * sizeof(*thread));

	for (int i=0;i<num_threads; i++){
		pthread_create(thread+i,NULL,&hello,NULL);
	}

	for (int i=0;i<num_threads; i++){
		pthread_join(thread[i],NULL);
	} 
}
