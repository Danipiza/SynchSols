#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <stdatomic.h>

/*
COMPILE: 
    gcc -pthread ticket.c 

EXECUTE: 
    ./a.out

It works similarly to a waiting queue in a supermarket 
where customers receive tickets and are served in order.
*/

_Atomic int atomic_ticket;
int tick;

// fetch_and_add operation can be also execute instead 
//  of an atomic operation of C std.
int get_ticket() { return atomic_ticket++; }


// -- lock functions -----------------------------------------------------------
void ticket_lock(int ticket) { while(tick!=ticket); }
// Release the lock
void ticket_unlock() { tick++; }


// -- thread function ----------------------------------------------------------
void* func(void* arg){
    long id=(long) arg;
    usleep(100);

    int ticket=get_ticket();
    ticket_lock(ticket);
    printf("Thread ID: %d\tTick: %d Ticket:%d\tSleeps 1 second\n",
        id, tick, ticket);
    sleep(1);
    ticket_unlock();
}

int main(int arg, char** argv){
    int num_threads=4;

    // -- init -----------------------------------------------------------------
    atomic_ticket=0;
    tick=0;


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