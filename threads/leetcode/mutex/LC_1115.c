#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct {
    int n;

    sem_t sem_foo;
    sem_t sem_bar;
} FooBar;

FooBar* fooBarCreate(int n) {
    FooBar* obj = (FooBar*) malloc(sizeof(FooBar));
    obj->n = n;
    

    sem_init(&obj->sem_foo,0,1);
    sem_init(&obj->sem_bar,0,0);    

    return obj;
}



void foo(FooBar* obj) {
    
    for (int i=0;i<obj->n;i++) {
        sem_wait(&obj->sem_foo);
        
        // printFoo() outputs "foo". Do not change or remove this line.
        printFoo();

        sem_post(&obj->sem_bar);        
    }
}

void bar(FooBar* obj) {
    
    for (int i=0;i<obj->n;i++) {
        sem_wait(&obj->sem_bar);

        // printBar() outputs "bar". Do not change or remove this line.
        printBar();

        sem_post(&obj->sem_foo);
    }
}

void fooBarFree(FooBar* obj) {
    sem_destroy(&obj->sem_foo);
    sem_destroy(&obj->sem_bar);
    
    free(obj);
}