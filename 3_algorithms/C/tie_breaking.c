#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <stdatomic.h>

#define NUM_THREADS 4

/*
COMPILE: 
    gcc -pthread tie_breaking.c 

EXECUTE: 
    ./a.out

NUM_THREADS layers. each thread goes through several layers
    and at each level, it checks if any other thread at that level
    has a higher priority (lower thread ID).
*/

int in[NUM_THREADS];    // layer position of each thread
int last[NUM_THREADS];  // last thread to enter the i-th layer

// -- lock functions -----------------------------------------------------------
void TB_lock(int id){
    
    for(int j=0;j<NUM_THREADS;j++){
        in[id]=j;
        last[j]=id;

        for(int k=0;k<NUM_THREADS;k++){
            if(k==id) continue;
            
            while(in[k]>in[id] && last[j]==id);
        }
        
    }   

}

// Release lock
void TB_unlock(int id){ in[id]=0; }


// -- thread function ----------------------------------------------------------
void* func(void* arg){
    long id=(long) arg;

    TB_lock(id);
    printf("Thread %d\tEnter the critical section. Sleeps 1 second.\n", id);
    sleep(1);
    TB_unlock(id);
    
}

int main(int argc, char** argv){

    // -- init -----------------------------------------------------------------    
    pthread_t threads[NUM_THREADS];

    
    for(long i=0;i<NUM_THREADS;i++){
        if(pthread_create(&threads[i], NULL, func, (void*) i)) {
            perror("Error while creating a thread");
            exit(EXIT_FAILURE);
        }
    }

    // -- wait -----------------------------------------------------------------
    for(int i=0;i<NUM_THREADS;i++){
        if(pthread_join(threads[i], NULL)) {
            perror("Error while waiting for a thread");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}