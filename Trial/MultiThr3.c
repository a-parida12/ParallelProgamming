#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

//the function for the thread
void* hello(void* args){
	int i=*(int*) args;

	int *c = (int*) malloc(sizeof(*c));

	*c= 1200*24+20+i;
	
	return (void*) c;
}

int main(){

	int num_threads=3;// specify the no. of threads in use
	int *count;

	pthread_t* thread = (pthread_t*) malloc(num_threads * sizeof(*thread));

	for (int i=0;i<num_threads; i++){
		count[i]=i;
		pthread_create(thread+i,NULL,&hello,count+i);
	}

	for (int i=0;i<num_threads; i++){
		int* c;
		pthread_join(thread[i],(void*)&c);
		printf("Result obtained by Thread %d is %d\n",i,*c);
	} 
}
