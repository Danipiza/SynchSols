#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <semaphore.h>

/*
// COMPILE
    gcc -pthread prod_cons_sem.c

// EXECUTE
    ./a.out
*/

#define BUFFER_SIZE 5
#define NUM_PRODUCERS 1
#define NUM_CONSUMERS 3

int buffer[BUFFER_SIZE];
int in=0, out=0; 

sem_t empty;   // counts empty buffer slots
sem_t full;    // counts full buffer slots
sem_t mutex;   // ensures mutual exclusion for buffer access

void* func_prod(void* arg) {
    int item;
    long id=(long) arg;

    while(1){
        // produce a random item
        item=rand()%100;  

        sem_wait(&empty); // empty slot

        sem_wait(&mutex); // mutual exclusion

        buffer[in]=item;  
        printf("Producer %ld produced: %d at %d\n", id, item, in);
        in=(in+1)%BUFFER_SIZE; 

        sem_post(&mutex); 
        sem_post(&full);  // signal that a new item is available
        
        printf("Producer sleeps\n");
        sleep(1);
    }

    return NULL;
}

void* consumer(void* arg) {
    int item;
    long id=(long) arg;

    while(1){        
        sem_wait(&full);     // full slot

        sem_wait(&mutex);    // mutual exclusion

        item=buffer[out];  
        printf("Consumer %ld consumed: %d from %d\n", id, item, out);
        out=(out+1)%BUFFER_SIZE; 

        sem_post(&mutex);    
        sem_post(&empty);   // signal that an item had been consumed  

        printf("Consummer sleeps\n");
        sleep(1);
    }

    return NULL;
}

int main() {
    pthread_t producers[NUM_PRODUCERS], consumers[NUM_CONSUMERS];

    sem_init(&empty, 0, BUFFER_SIZE); // init. all slots are empty
    sem_init(&full, 0, 0);            // init. no full slots
    
    // init. mutual exclusion, only one at the same time
    sem_init(&mutex, 0, 1);           

    for (long i=0;i<NUM_PRODUCERS;i++) {        
        if(pthread_create(&producers[i], NULL, func_prod, (void*)i)){
            perror("Error while creating decrement thread");
            exit(EXIT_FAILURE);
        }
    }
    for (long i=0;i<NUM_CONSUMERS;i++) {        
        if(pthread_create(&consumers[i], NULL, consumer, (void*)i)){
            perror("Error while creating decrement thread");
            exit(EXIT_FAILURE);
        }
    }

    for (int i=0;i<NUM_PRODUCERS;i++) {
        if(pthread_join(producers[i], NULL)){
            perror("Error while waiting for a producer thread");
            exit(EXIT_FAILURE);
        }             
    }
    for (int i=0;i<NUM_CONSUMERS;i++) {        
        if(pthread_join(consumers[i], NULL)){
            perror("Error while waiting for a consummer thread");
            exit(EXIT_FAILURE);
        }
    }


    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    return 0;
}
