#include "histogram.h"

#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "names.h"

int a=0;

struct pthread_args{

	char *buffer;
	int* histogram;
		
};

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* threadOperator(void* ptr){

	struct pthread_args *thread_arg = (struct pthread_args*) ptr;


	char current_word[20] = "";
	int c = 0;

	pthread_mutex_lock(&mutex);
		//Critical Region here

//	for (int i=a; thread_arg->buffer[i]!=TERMINATOR; i++){

//		if(isalpha(thread_arg->buffer[i]) && i%CHUNKSIZE!=0 ){
//			current_word[c++] = thread_arg->buffer[i];
//		} 

//		else {
//			current_word[c] = '\0';
//			int res = getNameIndex(current_word); // new hash-function
//			if (res != -1)
// 				thread_arg->histogram[res]++;
// 			c = 0;
//		}
//	a=i;
//	}

	pthread_mutex_unlock(&mutex);

	return NULL;

}

void get_histogram(char *buffer, int* histogram, int num_threads){

	pthread_t *thread;
	struct pthread_args *argument;

	thread=(pthread_t*)malloc(num_threads * sizeof(*thread));
	argument=(struct pthread_args*)malloc(num_threads * sizeof(*argument));


	for (int i=0;i<num_threads; i++){

		argument[i].buffer=buffer;
		argument[i].histogram=histogram;
		
		pthread_create(thread+i,NULL,&threadOperator,argument+i);
	}

	for (int i=0;i<num_threads; i++){
		pthread_join(thread[i] , NULL);
		
	}

	free(thread);
	free(argument);

}
