#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <stdatomic.h>

/*
Is the same program as the ""./intro/race_cond.c" code.
Increment a global variable multiple times by different threads.
In this case there is no need for a mutex delete possible race conditions.

// COMPILE
    gcc -pthread wait_free.c

// EXECUTE
    ./a.out
*/


_Atomic int counter=0; // global atomic counter

// (Wait-free) increment
void increment_counter() { atomic_fetch_add(&counter, 1); }

// (Wait-free) read
int get_counter_value() { return atomic_load(&counter); }


void* func(void* arg) {    

    for (int i=0;i<100;i++) {
        usleep(1); // sleep for 1 microsecond
        increment_counter();
    }

    return NULL;
}

int main() {
    int num_threads=100;

    pthread_t threads[num_threads];

    for (long i=0;i<num_threads;i++) {        
        if(pthread_create(&threads[i], NULL, func, NULL)){
            perror("Error while creating a push thread");
            exit(EXIT_FAILURE);
        }         
    } 


    for (int i=0;i<num_threads;i++) {        
        if(pthread_join(threads[i], NULL)){
            perror("Error while waiting for a push thread");
            exit(EXIT_FAILURE);
        }         
    }

    // print the final counter value
    printf("Final Counter Value: %d\n", get_counter_value());  
    // should be 10000 (100 threads * 100 increments each)


    return 0;
}
