#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct {
    int n;
    int tick;
    sem_t sem_zero;
    sem_t sem_odd;
    sem_t sem_even;
} ZeroEvenOdd;

ZeroEvenOdd* zeroEvenOddCreate(int n) {
    ZeroEvenOdd* obj = (ZeroEvenOdd*) malloc(sizeof(ZeroEvenOdd));
    obj->n=n;
    obj->tick=1;

    sem_init(&obj->sem_zero,0,1);
    sem_init(&obj->sem_odd,0,0);
    sem_init(&obj->sem_even,0,0);

    return obj;
}

// You may call global function `void printNumber(int x)`
// to output "x", where x is an integer.

void zero(ZeroEvenOdd* obj) {
    while(1){
        sem_wait(&obj->sem_zero);

        if(obj->tick>obj->n) {
            sem_post(&obj->sem_odd);
            sem_post(&obj->sem_even);
            break;
        }

        
        printNumber(0);
        
        if(obj->tick%2!=0) sem_post(&obj->sem_odd);            
        else sem_post(&obj->sem_even);
    }
}

void even(ZeroEvenOdd* obj) {
    while(1){
        sem_wait(&obj->sem_even);

        if(obj->tick>obj->n) break;

        printNumber(obj->tick++);

        
        sem_post(&obj->sem_zero);
    }
}

void odd(ZeroEvenOdd* obj) {
    while(1){
        sem_wait(&obj->sem_odd);

        if(obj->tick>obj->n) break;

        printNumber(obj->tick++);
        
        sem_post(&obj->sem_zero);
    }
}

void zeroEvenOddFree(ZeroEvenOdd* obj) {
    sem_destroy(&obj->sem_zero);
    sem_destroy(&obj->sem_odd);
    sem_destroy(&obj->sem_even);
    free(obj);
}