#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <stdatomic.h>

#define NUM_THREADS 4

/*
COMPILE: 
    gcc -pthread bakery.c 

EXECUTE: 
    ./a.out

Each thread enters the queue, ask for the greatest turn value
    and his new value is the greatest turn + 1.

Waits for its turn.
*/

int turn[NUM_THREADS];

// -- lock functions -----------------------------------------------------------
void bakery_lock(int id){
    
    for(int j=0;j<NUM_THREADS;j++){
        if(j==id) continue;
        while(turn[j]==0 || turn[id]>turn[j]);
    }


}

void bakery_unlock(int id){ turn[id]=NUM_THREADS+1; }

/*
Args:
    a (int) : Thread 1 turn.
    b (int) : Thread 1 id.
    c (int) : Thread 2 turn.
    d (int) : Thread 2 id.
*/
int compare_thread_conditions(int a, int b, int c, int d){
    return ((a<c || (a==c && b<d)) ? 1:0);
}

int get_number(){
    int ret=1;

    for(int i=0;i<NUM_THREADS;i++){
        if(ret<turn[i]) ret=turn[i];
    }
    
    return ret+1;
}

// -- thread function ----------------------------------------------------------
void* func(void* arg){
    long id=(long) arg;

    turn[id]=1;
    turn[id]=get_number();

    bakery_lock(id);
    printf("Thread %d\tEnter the critical section. Sleeps 1 second\n",id);
    sleep(1);
    bakery_unlock(id);

}

int main(int argc, char** argv){
    
    // -- init -----------------------------------------------------------------
    for(int i=0;i<NUM_THREADS;i++) turn[i]=0;

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