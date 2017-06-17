#include "histogram.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include "names.h"
#define CHUNK 401 // number of characters
int *global_hist; // for each thread to do reduction
char *common_buffer; // start pointer of the names
int residual;
int job_index;

pthread_mutex_t mutex_variable = PTHREAD_MUTEX_INITIALIZER; // mutex for tasks
pthread_mutex_t mutex_count = PTHREAD_MUTEX_INITIALIZER; // mutex for reduction

void * Consumer(void * arg) {
    int local_hist[NNAMES]; 
    
    for (int i = 0; i < NNAMES; i ++)
        local_hist[i] = 0;
    
    int count = 0;
    unsigned int start, end;
    // local start and end points
    while(1) {
        pthread_mutex_lock(&mutex_variable);
        count = job_index;
        job_index++;
        pthread_mutex_unlock(&mutex_variable);
        start = count * CHUNKSIZE;
        if (count < CHUNK) {
            end = (count + 1) * CHUNKSIZE;
        } else if (count == CHUNK) {
            end = count * CHUNKSIZE + residual;
        } else
            break;
        char current_word[20] = "";
        int c = 0;
        for (unsigned int i = start; i < end; i++) {
            
            if (isalpha(common_buffer[i])) {
              
                current_word[c++] = common_buffer[i];
            } else {
                 
                current_word[c] = '\0';
                int res = getNameIndex(current_word);
                if (res != -1)
                    local_hist[res]++;
                c = 0;
            }
        }
    }
   
    for (int i = 0; i < NNAMES; i++)
    {
     pthread_mutex_lock(&mutex_count);
        global_hist[i] += local_hist[i];
        pthread_mutex_unlock(&mutex_count);
     }         
   
    return NULL;
}


void get_histogram(char *buffer, int* histogram, int num_threads) {
    common_buffer = buffer;
    job_index = 0;
    global_hist = histogram;
    pthread_t *thread;
    thread = (pthread_t*)malloc(num_threads * sizeof(*thread));
    for (int i = 0; i < num_threads; i ++) {
        pthread_create(thread+i, NULL, &Consumer, NULL);
    }
    
    int my_res = 5*CHUNK;
    while (common_buffer[CHUNK*CHUNKSIZE+my_res] != TERMINATOR)
        my_res ++;
    residual = my_res;
    
    for (int i = 0; i < num_threads; i ++)
        pthread_join(thread[i], NULL);
    free(thread);
}
