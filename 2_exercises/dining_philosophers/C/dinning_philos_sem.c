#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <semaphore.h>

#define NUM_THREADS 5  // number of threads
sem_t forks[NUM_THREADS];

/*
// COMPILE
    gcc -pthread dinning_philos.c

// EXECUTE
    ./a.out
*/


void* philosopher(void* arg) {
    int left, right;

    int id=*(int*)arg;
    
    
    
    // -- deadlocks ------------------------------------------------------------
    // this is made to erase deadlocks posibilities.
    if(id!=NUM_THREADS-1){
        left  =id;
        right =(id+1)%NUM_THREADS;
    }
    else {
        left  =(id+1)%NUM_THREADS;
        right =id;
    }

    //printf("Philo %d\tleft=%d\tright=%d\n",i,left,right);

    usleep(10);

    
    while (1) {
        printf("Philosopher %d is thinking\n", id);
        sem_wait(&forks[left]); 
        sem_wait(&forks[right]);
        //pthread_mutex_lock(&forks[left]);
        //pthread_mutex_lock(&forks[right]);
        printf("Philosopher %d is eating\n", id);
        sem_post(&forks[left]);
        sem_post(&forks[right]);
        //pthread_mutex_unlock(&forks[left]);
        //pthread_mutex_unlock(&forks[right]);
        sleep(1);
    }
}

int main() {
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    for(int i=0;i<NUM_THREADS;i++) sem_init(&forks[i], 0, 1); 

    for (int i=0;i<NUM_THREADS;i++) {        
        ids[i]=i;
        if(pthread_create(&threads[i], NULL, philosopher, &ids[i])){
            perror("Error while creating a thread");
            exit(EXIT_FAILURE);
        }         
    }

    for (int i=0;i<NUM_THREADS;i++) {    
        if(pthread_join(threads[i], NULL)){
            perror("Error while waiting for a thread");
            exit(EXIT_FAILURE);
        }   
    }

    for (int i=0;i<NUM_THREADS;i++) sem_destroy(&forks[i]);

    return 0;
}
