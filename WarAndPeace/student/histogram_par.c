#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "histogram.h"

int total_thread;
int nBlockThread;

struct pthread_args{

	long thread_no;
	block_t *blocks;
	int m_histogram[NNAMES];
		
};

void* Checkword(void* ptr){

	char current_word[20] = "";
	int c = 0;

	struct pthread_args *arg = (pthread_args *)ptr;

	int start= arg->thread_no*nBlockThread;
	int end=start+nBlockThread;

	if ((arg->thread_no)+1==total_thread)
			end=nBlockThread*total_thread;

	for (int i = start; i < end; i++) {

		for (int j = 0; j < BLOCKSIZE;j++) {

			// if the character is an alphabet store it to form a word.
			if(isalpha(arg->blocks[i][j])){
				current_word[c++] = arg->blocks[i][j];
			}

			// if the character is not an alphabet it is the end of a word.
			// Compare the word with the list of names.
			else {
				current_word[c] = '\0';
				for(int k = 0; k < NNAMES; k++)	{
					if(current_word[0]==names[k][0]&&current_word[1]==names[k][1]){

						if(!strcmp(current_word, names[k])){
							arg->m_histogram[k]++;
						}
					}
				}
			c = 0;
			}

		}
	}

	return NULL;
}



void get_histogram(int nBlocks, block_t *blocks, histogram_t histogram, int num_threads)
{
	total_thread=num_threads;
	nBlockThread=nBlocks/(total_thread);

	pthread_t *thread;
	struct pthread_args *arg;

	thread=(pthread_t*)malloc(num_threads * sizeof(*thread));
	arg=(struct pthread_args*) malloc(num_threads*sizeof(*arg));


	for (int i=0;i<num_threads; i++){
		arg[i].thread_no=i;
		arg[i].blocks=blocks;

		for (int k = 0; k < NNAMES; k ++){
        arg[i].m_histogram[k] = 0;
		}
	
		pthread_create(thread+i,NULL,&Checkword,arg+i);
	}

	for (int i=0;i<num_threads; i++){
		pthread_join(thread[i] , NULL);
		
		for(int k=0;k<NNAMES; k++){
			histogram[k]+=arg[i].m_histogram[k];
		}
	}

	free(thread);
	free(arg);
}
