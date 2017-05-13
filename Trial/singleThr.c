#include<stdio.h>
#include<pthread.h>

//the function for the thread
void* hello(void* args){
	printf("Hello World from Pthread\n");
	
	return NULL;
}

int main(){
	//intialise our pthread
	pthread_t thread;
	
	//create the thread and link to the required function
	pthread_create(&thread,NULL,&hello,NULL);

	printf("Hello World from main\n");

	pthread_join(thread,NULL);
}
