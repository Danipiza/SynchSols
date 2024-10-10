
#define _POSIX_C_SOURCE 200112L // remove possible

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>


pthread_barrier_t barrier;

void* simple_func(void* arg){
    long idx=(long) arg;
    sleep(idx);
    printf("Thread %d arrived at the barrier\n", idx);
    pthread_barrier_wait(&barrier);
    printf("Thread %d exit the barrier\n", idx);
}

void simple_barrier(){
    int num_threads=5;

    pthread_t threads[num_threads];
    pthread_barrier_init(&barrier, NULL, num_threads);
    for(long i=0;i<num_threads;i++){
       if(pthread_create(&threads[i], NULL, &simple_func, (void*) i)){
            perror("Error while creating a thread");
            exit(EXIT_FAILURE);
        }
    }

    for(int i=0;i<10;i++){
        if(pthread_join(threads[i], NULL)){
            perror("Error while waiting for a thread");
            exit(EXIT_FAILURE);
        }
    }


    pthread_barrier_destroy(&barrier);
}

int main(int argc, char** argv){
    simple_barrier();

    return 0;
}