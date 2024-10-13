#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct {
    int n;
    int zero_tick;
    int count;
    
    pthread_mutex_t mut;
    pthread_cond_t cond;

    int end;    
} ZeroEvenOdd;

ZeroEvenOdd* zeroEvenOddCreate(int n) {
    ZeroEvenOdd* obj = (ZeroEvenOdd*) malloc(sizeof(ZeroEvenOdd));
    obj->n = n;

    obj->zero_tick=1;
    obj->count=1;
    pthread_mutex_init(&obj->mut, NULL);
    pthread_cond_init(&obj->cond, NULL);

    obj->end=0;
    
    return obj;
}

// You may call global function `void printNumber(int x)`
// to output "x", where x is an integer.

void zero(ZeroEvenOdd* obj) {
    while(1){
        pthread_mutex_lock(&obj->mut);
        while(!obj->zero_tick&&!obj->end) pthread_cond_wait(&obj->cond, &obj->mut);

        if(obj->end){                
            pthread_mutex_unlock(&obj->mut);
            //pthread_cond_broadcast(&cond);

            break;
        }
        
        printNumber(0);

        obj->zero_tick=0;
        pthread_mutex_unlock(&obj->mut);
        pthread_cond_broadcast(&obj->cond);
    }
}

void even(ZeroEvenOdd* obj) {
    while(1){
        pthread_mutex_lock(&obj->mut);
        while((obj->zero_tick||obj->count%2!=0)&&!obj->end) pthread_cond_wait(&obj->cond, &obj->mut);

        if(obj->end) {
            pthread_mutex_unlock(&obj->mut);
            break;
        }

        printNumber(obj->count);

        obj->count++;
        obj->zero_tick=1;

        if(obj->count>obj->n){
            obj->end=1;
            pthread_mutex_unlock(&obj->mut);
            pthread_cond_broadcast(&obj->cond);
            break;
        }

        pthread_mutex_unlock(&obj->mut);
        pthread_cond_broadcast(&obj->cond);
    }
}

void odd(ZeroEvenOdd* obj) {
     while(1){
        pthread_mutex_lock(&obj->mut);
        while((obj->zero_tick||obj->count%2==0)&&!obj->end) pthread_cond_wait(&obj->cond, &obj->mut);

        if(obj->end) {
            pthread_mutex_unlock(&obj->mut);
            break;
        }

        printNumber(obj->count);

        obj->count++;
        obj->zero_tick=1;

        if(obj->count>obj->n){
            obj->end=1;
            pthread_mutex_unlock(&obj->mut);
            pthread_cond_broadcast(&obj->cond);
            break;
        }

        pthread_mutex_unlock(&obj->mut);
        pthread_cond_broadcast(&obj->cond);
    }
}

void zeroEvenOddFree(ZeroEvenOdd* obj) {
    
}