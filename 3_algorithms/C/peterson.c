#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <stdatomic.h>


/*
COMPILE: 
    gcc -pthread peterson.c 

EXECUTE: 
    ./a.out

Peterson's Algorithm, which handles mutual exclusion between two threads. 
This code tries to guarantee that only one thread is in the critical section. 

The last thread to modify the atomic global variable "turn" is the one
    that waits for the other thread to finish its execution.
*/

// atomic_int. also accepted
_Atomic int want[2];  // indicates if a trhead wants to enter the critical section
_Atomic int turn;     // indicates the whose turn it is


// -- lock functions -----------------------------------------------------------
void peterson_lock(int id) {
    int other_thread=1-id;
    
    want[id]=1;
    turn=other_thread;
    
    while (want[other_thread]&&turn==other_thread);           
}

// Release the lock
void peterson_unlock(int id) { want[id]=0; }


// -- thread function ----------------------------------------------------------
void *func(void *arg) {
    int id=(long)arg;

    peterson_lock(id);
    printf("Thread %d\tEnter the critical section. Sleeps 1 second.\n", id);
    sleep(1);
    peterson_unlock(id);
    return NULL;
}

int main() {
    int num_threads=2;

    // -- init -----------------------------------------------------------------
    want[0]=want[1]=0;
    turn=0;
    

    pthread_t threads[num_threads];

    for(long i=0;i<num_threads;i++){
        if(pthread_create(&threads[i], NULL, func, (void*) i)){
            perror("Error while creating a thread");
            exit(EXIT_FAILURE);
        }
    }
    
    // -- wait -----------------------------------------------------------------
    for(long i=0;i<num_threads;i++){
        if(pthread_join(threads[i], NULL)){
            perror("Error while waiting for a thread");
            exit(EXIT_FAILURE);
        }
    }
    
    return 0;
}
