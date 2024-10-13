#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>



typedef struct {
    int n;
    int tick;
    int end;
    pthread_mutex_t mut;
    pthread_cond_t cond;
} FizzBuzz;

FizzBuzz* fizzBuzzCreate(int n) {
    FizzBuzz* obj = (FizzBuzz*) malloc(sizeof(FizzBuzz));
    obj->n=n;

    obj->tick=1;
    obj->end=0;
    pthread_mutex_init(&obj->mut, NULL);
    pthread_cond_init(&obj->cond, NULL);

    return obj;
}

// printFizz() outputs "fizz".
void fizz(FizzBuzz* obj) {
    while(1) {
        pthread_mutex_lock(&obj->mut);

        while((obj->tick%3!=0 || obj->tick % 5==0) && !obj->end) {
            pthread_cond_wait(&obj->cond, &obj->mut);
        }

        if (obj->tick>obj->n || obj->end) {
            obj->end=1;
            pthread_cond_broadcast(&obj->cond);
            pthread_mutex_unlock(&obj->mut);
            break;
        }

        printFizz();
        obj->tick++;
        pthread_cond_broadcast(&obj->cond);
        pthread_mutex_unlock(&obj->mut);
    }
}

// printBuzz() outputs "buzz".
void buzz(FizzBuzz* obj) {
    while(1) {
        pthread_mutex_lock(&obj->mut);

        while((obj->tick%5!=0 || obj->tick%3==0) && !obj->end) {
            pthread_cond_wait(&obj->cond, &obj->mut);
        }

        if (obj->tick>obj->n || obj->end) {
            obj->end=1;
            pthread_cond_broadcast(&obj->cond);
            pthread_mutex_unlock(&obj->mut);
            break;
        }

        printBuzz();
        obj->tick++;
        pthread_cond_broadcast(&obj->cond);
        pthread_mutex_unlock(&obj->mut);
    }
}

// printFizzBuzz() outputs "fizzbuzz".
void fizzbuzz(FizzBuzz* obj) {
    while(1) {
        pthread_mutex_lock(&obj->mut);

        while((obj->tick%3!=0 || obj->tick%5!=0) && !obj->end) {
            pthread_cond_wait(&obj->cond, &obj->mut);
        }

        if (obj->tick>obj->n || obj->end) {
            obj->end=1;
            pthread_cond_broadcast(&obj->cond);
            pthread_mutex_unlock(&obj->mut);
            break;
        }

        printFizzBuzz();
        obj->tick++;
        pthread_cond_broadcast(&obj->cond);
        pthread_mutex_unlock(&obj->mut);
    }
}

// You may call global function `void printNumber(int x)` 
// to output "x", where x is an integer.
void number(FizzBuzz* obj) {
    while(1) {
        pthread_mutex_lock(&obj->mut);

        while((obj->tick%3==0 || obj->tick%5==0) && !obj->end) {
            pthread_cond_wait(&obj->cond, &obj->mut);
        }

        if (obj->tick>obj->n || obj->end) {
            obj->end=1;
            pthread_cond_broadcast(&obj->cond);
            pthread_mutex_unlock(&obj->mut);
            break;
        }

        printNumber(obj->tick);
        obj->tick++;
        pthread_cond_broadcast(&obj->cond);
        pthread_mutex_unlock(&obj->mut);
    }
}

void fizzBuzzFree(FizzBuzz* obj) {
    pthread_mutex_destroy(&obj->mut);
    pthread_cond_destroy(&obj->cond);
    free(obj);
}
