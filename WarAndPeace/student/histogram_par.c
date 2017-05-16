#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "histogram.h"

struct pthread_args{
	int total_thread;
	long thread_no;
	int nBlocks;
	block_t *blocks;
	int* histogram;
};


void* Checkword(void* ptr){

	char current_word[20] = "";
	int c = 0;
	struct pthread_args *arg = (pthread_args *)ptr;
	
	int Thr_no=arg->thread_no;
	int nBlocks=arg->nBlocks;
	block_t *blocks=arg->blocks;
	int* histogram=arg->histogram;


	printf("Hello from Thread %d\n",Thr_no);
	printf("block size %d\n",nBlocks);

	int nBlockThread=nBlocks/(arg->total_thread);
	printf("block size %d\n",nBlockThread);
	for (int i = Thr_no*nBlockThread; i <Thr_no*nBlockThread+nBlockThread; i++) {
		printf("i %d\n",i);
		printf("%d,BLOCKSIZE",BLOCKSIZE);
		for (int j = 0; j < BLOCKSIZE; j++) {
			//printf("j %d\n",i);
			// if the character is an alphabet store it to form a word.
			if(isalpha(blocks[i][j])){
				current_word[c++] = blocks[i][j];
			}

			// if the character is not an alphabet it is the end of a word.
			// Compare the word with the list of names.
			else {
				current_word[c] = '\0';
				for(int k = 0; k < NNAMES; k++)	{
					if(!strcmp(current_word, names[k]))
						histogram[k]++;
				}
			}
		}
	}
	arg->histogram=histogram;
	return NULL;
}



void get_histogram(int nBlocks, block_t *blocks, int* histogram, int num_threads)
{

	pthread_t *thread;
	struct pthread_args *arg;
	thread=(pthread_t*)malloc(num_threads * sizeof(*thread));
	arg=(struct pthread_args*) malloc(num_threads*sizeof(*arg));
	
	
	for (int i=0;i<num_threads; i++){
		arg[i].total_thread=num_threads;
		arg[i].thread_no=i;
		arg[i].nBlocks=nBlocks;
		arg[i].blocks=blocks;
		arg[i].histogram=histogram;
		printf("Thread created\n");
		pthread_create(thread+i,NULL,&Checkword,arg+i);
	}

	for (int i=0;i<num_threads; i++){
		pthread_join(thread[i] , NULL);
		printf("Thread Joined\n");
		for(int k = 0; k < NNAMES; k++){
			histogram[k]=histogram[k]+arg[i].histogram[k];
		}

	}

	free(thread);
	free(arg);
}
