#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <semaphore.h>

/*
// COMPILE
    gcc -pthread semaphore.c

// EXECUTE
    ./a.out
*/

#define NUM_ITER 10

int counter=0;
sem_t semaphore;

void* increment(void* arg) {

    for (int i=0;i<NUM_ITER;i++) {
        sem_wait(&semaphore); // wait (decrement)

        counter++;            
        printf("Incremented: %d\n", counter);

        sem_post(&semaphore); // signal (increment)
    }

    return NULL;
}

void* decrement(void* arg) {

    for (int i=0;i<NUM_ITER;i++) {
        sem_wait(&semaphore); // wait (decrement)

        counter--;            
        printf("Decremented: %d\n", counter);

        sem_post(&semaphore); // signal (increment)
    }

    return NULL;
}

int main() {
    pthread_t inc_thread, dec_thread;

    /*
    ARGS:
        sem_t:
        int: semaphore used for a number or process.
        int: initial value=1
    */
    sem_init(&semaphore, 0, 1); 
    
    
    if(pthread_create(&inc_thread, NULL, increment, NULL)){
        perror("Error while creating increment thread");
        exit(EXIT_FAILURE);
    }
    if(pthread_create(&dec_thread, NULL, decrement, NULL)){
        perror("Error while creating decrement thread");
        exit(EXIT_FAILURE);
    }

    
    if(pthread_join(inc_thread, NULL)){
        perror("Error while waiting for increment thread");
        exit(EXIT_FAILURE);
    }     
    if(pthread_join(dec_thread, NULL)){
        perror("Error while waiting for decrement thread");
        exit(EXIT_FAILURE);
    } 

    sem_destroy(&semaphore); 

    return 0;
}
