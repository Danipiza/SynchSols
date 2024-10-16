#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// threads A, B, C, D
#define NUM_THREADS 4

typedef struct {
    int n;
    int tick;
    int end;
    sem_t sem_mutex;
    
} FizzBuzz;

FizzBuzz* fizzBuzzCreate(int n) {
    FizzBuzz* obj = (FizzBuzz*) malloc(sizeof(FizzBuzz));
    obj->n=n;

    obj->tick=1;
    obj->end=0;
    sem_init(&obj->sem_mutex, 0, 1); // binary semaphore
    

    return obj;
}

// printFizz() outputs "fizz".
void fizz(FizzBuzz* obj) {
    while(1) {
        sem_wait(&obj->sem_mutex);

        while((obj->tick%3!=0 || obj->tick%5==0)&&!obj->end) {
            sem_post(&obj->sem_mutex);
            sem_wait(&obj->sem_mutex);                       
        }

        if (obj->tick > obj->n || obj->end) {
            obj->end=1;
            for(int i=0;i<NUM_THREADS;i++) sem_post(&obj->sem_mutex);           
            
            break;
        }

        printFizz();
        obj->tick++;
        for(int i=0;i<NUM_THREADS;i++) sem_post(&obj->sem_mutex);
    }
}

// printBuzz() outputs "buzz".
void buzz(FizzBuzz* obj) {
    while(1) {
        sem_wait(&obj->sem_mutex);

        while((obj->tick % 5!=0 || obj->tick%3==0) && !obj->end) {
            sem_post(&obj->sem_mutex);
            sem_wait(&obj->sem_mutex);
        }

        if (obj->tick>obj->n || obj->end) {
            obj->end=1;
            for(int i=0;i<NUM_THREADS;i++) sem_post(&obj->sem_mutex);

            break;
        }

        printBuzz();
        obj->tick++;
        for(int i=0;i<NUM_THREADS;i++) sem_post(&obj->sem_mutex);
    }
}

// printFizzBuzz() outputs "fizzbuzz".
void fizzbuzz(FizzBuzz* obj) {
    while(1) {
        sem_wait(&obj->sem_mutex);

        while((obj->tick % 3!=0 || obj->tick%5!=0) && !obj->end) {
            sem_post(&obj->sem_mutex);
            sem_wait(&obj->sem_mutex);
        }

        if (obj->tick>obj->n || obj->end) {
            obj->end=1;
            for(int i=0;i<NUM_THREADS;i++) sem_post(&obj->sem_mutex);
            break;
        }

        printFizzBuzz();
        obj->tick++;
        for(int i=0;i<NUM_THREADS;i++) sem_post(&obj->sem_mutex);
    }
}

// You may call global function `void printNumber(int x)` 
// to output "x", where x is an integer.
void number(FizzBuzz* obj) {
    while(1) {
        sem_wait(&obj->sem_mutex);

        while((obj->tick%3== 0 || obj->tick%5==0) && !obj->end) {
            sem_post(&obj->sem_mutex);
            sem_wait(&obj->sem_mutex);
        }

        if (obj->tick>obj->n || obj->end) {
            obj->end=1;
            for(int i=0;i<NUM_THREADS;i++) sem_post(&obj->sem_mutex);
            break;
        }

        printNumber(obj->tick);
        obj->tick++;
        for(int i=0;i<NUM_THREADS;i++) sem_post(&obj->sem_mutex);
    }
}

void fizzBuzzFree(FizzBuzz* obj) {
    sem_destroy(&obj->sem_mutex);    
    free(obj);
}
