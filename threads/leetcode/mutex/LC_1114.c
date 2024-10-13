#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


typedef struct {
    // User defined data may be declared here.
    int x;
    pthread_mutex_t mut;
    pthread_cond_t cond;
} Foo;

Foo* fooCreate() {
    Foo* obj = (Foo*) malloc(sizeof(Foo));
    obj->x=1;
    // Initialize user defined data here.
    pthread_mutex_init(&obj->mut, NULL);
    pthread_cond_init(&obj->cond, NULL);
    return obj;
}

void first(Foo* obj) {
    pthread_mutex_lock(&obj->mut);
    while(obj->x!=1) pthread_cond_wait(&obj->cond, &obj->mut);

    obj->x++;
    // printFirst() outputs "first". Do not change or remove this line.
    printFirst();
    pthread_mutex_unlock(&obj->mut); 
    pthread_cond_broadcast(&obj->cond);
}

void second(Foo* obj) {
    pthread_mutex_lock(&obj->mut);
    while(obj->x!=2) pthread_cond_wait(&obj->cond, &obj->mut);

    obj->x++;
    // printSecond() outputs "second". Do not change or remove this line.
    printSecond();
    pthread_mutex_unlock(&obj->mut); 
    pthread_cond_broadcast(&obj->cond);
}

void third(Foo* obj) {
    pthread_mutex_lock(&obj->mut);
    while(obj->x!=3) pthread_cond_wait(&obj->cond, &obj->mut);
    
    // printThird() outputs "third". Do not change or remove this line.
    printThird();
    pthread_mutex_unlock(&obj->mut);    
}

void fooFree(Foo* obj) {
    // User defined data may be cleaned up here.
    pthread_mutex_destroy(&obj->mut);
    pthread_cond_destroy(&obj->cond);
    free(obj);
}