#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct {
    // User defined data may be declared here.
    sem_t semaphores[3];
} Foo;

Foo* fooCreate() {
    Foo* obj = (Foo*) malloc(sizeof(Foo));
    int init_vals[3]={1,0,0};
    // Initialize user defined data here.
    for(int i=0;i<3;i++){
        sem_init(&obj->semaphores[i], 0,init_vals[i]);        
    }
    return obj;
}

void first(Foo* obj) {
    
    // printFirst() outputs "first". Do not change or remove this line.
    printFirst();
    sem_post(&obj->semaphores[1]);
}

void second(Foo* obj) {
    sem_wait(&obj->semaphores[1]);
    // printSecond() outputs "second". Do not change or remove this line.
    printSecond();
    sem_post(&obj->semaphores[2]);
}

void third(Foo* obj) {
    sem_wait(&obj->semaphores[2]);
    // printThird() outputs "third". Do not change or remove this line.
    printThird();
}

void fooFree(Foo* obj) {
    // User defined data may be cleaned up here.
    for(int i=0;i<3;i++) sem_destroy(&obj->semaphores[i]);
    free(obj);
}