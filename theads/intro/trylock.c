#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// COMPILE:
// gcc -pthread intro.c

// EXECUTE
// ./a.out

#define NUM_THREADS 3

pthread_mutex_t mut;

void* func(void *arg){
    long idx=(long)arg;
    usleep(10);

    if(pthread_mutex_trylock(&mut)==0){
        printf("Thread %d, GOT the lock\n",idx);
        usleep(10);
        pthread_mutex_unlock(&mut);
    }
    else {  
        printf("Thread %d, did not get the lock\n",idx);
    }
}


int main(int argc, char** argv) {
    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&mut, NULL);

    for(long i=0;i<NUM_THREADS;i++){
        if(pthread_create(&threads[i], NULL, &func, (void *) i) != 0){            
            perror("Error while creating a thread");
            exit(EXIT_FAILURE);
        }
    }

    for(long i=0; i<NUM_THREADS; i++){
        if(pthread_join(threads[i], NULL) != 0){            
            perror("Error while waiting for a thread\n");
            exit(EXIT_FAILURE);
        }
    }

    pthread_mutex_destroy(&mut);

    return 0;
}