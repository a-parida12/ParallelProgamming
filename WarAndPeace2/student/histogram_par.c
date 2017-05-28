#include "histogram.h"

#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>

#include "names.h"

char *global_buffer;
int residual;
int curTask_index;
int CHUNK;

pthread_mutex_t mutex_variable = PTHREAD_MUTEX_INITIALIZER;

void * ThreadWorker(void * arg) {
	
    int local_hist[NNAMES];
    int* m_hist = (int*)arg;
    int count = 0;
    unsigned int start, end;
    
    for (int i = 0; i < NNAMES; i ++)
        local_hist[i] = 0;

    while(1) {
    
        pthread_mutex_lock(&mutex_variable);
        count = curTask_index;
        curTask_index++;
        pthread_mutex_unlock(&mutex_variable);
        
        if (count < (CHUNK-1)) {
            start = count * CHUNKSIZE;
            end = (count + 1) * CHUNKSIZE;
        } else if (count == (CHUNK-1)) {
            start = count * CHUNKSIZE;
            end = count * CHUNKSIZE + residual;
        } else
            break;
        
        char current_word[20] = "";
        int c = 0;
        for (unsigned int i = start; i < end; i++) {
                if (isalpha(global_buffer[i])) {
	            current_word[c++] = global_buffer[i];
	        } else {
	            current_word[c] = '\0';
	            int res = getNameIndex(current_word);
	            if (res != -1)
	                local_hist[res]++;
	            c = 0;
	        }
        }
    }
    
    for (int i=0; i<NNAMES; i++)
    {
    	m_hist[i] = local_hist[i];
    }
    
    return NULL;
}


void get_histogram(char *buffer, int* histogram, int num_threads) {
    global_buffer = buffer;
    CHUNK = strlen(buffer) / CHUNKSIZE;
	curTask_index=0;
	
    pthread_t *thread;
    thread = (pthread_t*)malloc(num_threads * sizeof(*thread));

	int** hist = (int**)malloc(num_threads*sizeof(int*));
    for (int i=0; i<num_threads; i++)
      	hist[i] = (int*)calloc(NNAMES,sizeof(int));
    	
    for (int i = 0; i < num_threads; i ++) 
        pthread_create(thread+i, NULL,&ThreadWorker, hist[i]);
 
    residual = 0;
    
    while (global_buffer[(CHUNK-1)*CHUNKSIZE+residual] != TERMINATOR)
        residual++;
        
    for (int i = 0; i < num_threads; i ++)
        pthread_join(thread[i], NULL);
    
	for (int i=0; i<num_threads; i++)
        for (int j=0; j<NNAMES; j++)
     	    histogram[j] += hist[i][j];
    
    free(thread);
}

