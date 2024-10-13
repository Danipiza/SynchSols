#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct {
    // User defined data may be declared here.
    int h;
    int o;

    sem_t sem_global;
    sem_t sem_h;
    sem_t sem_o;
} H2O;

H2O* h2oCreate() {
    H2O* obj = (H2O*) malloc(sizeof(H2O));

    obj->h=2;
    obj->o=1;

    sem_init(&obj->sem_global, 0, 2);
    sem_init(&obj->sem_h, 0, 2);
    sem_init(&obj->sem_o, 0, 1);
    
    // Initialize user defined data here.
    
    return obj;
}

void reset_values(H2O* obj){
    if(obj->h==0&&obj->o==0){
        obj->h=2;
        obj->o=1;
        sem_post(&obj->sem_h);
        sem_post(&obj->sem_h);
        sem_post(&obj->sem_o);
    }
}

void hydrogen(H2O* obj) {
    sem_wait(&obj->sem_h);
    // releaseHydrogen() outputs "H". Do not change or remove this line.
    releaseHydrogen();

    sem_wait(&obj->sem_global);
    obj->h--;
    reset_values(obj);
    sem_post(&obj->sem_global);

}

void oxygen(H2O* obj) {
    sem_wait(&obj->sem_o);
    // releaseOxygen() outputs "O". Do not change or remove this line.
    releaseOxygen();
    sem_wait(&obj->sem_global);
    obj->o--;
    reset_values(obj);
    sem_post(&obj->sem_global);
}

void h2oFree(H2O* obj) {
    // User defined data may be cleaned up here.
    sem_destroy(&obj->sem_global);
    sem_destroy(&obj->sem_h);
    sem_destroy(&obj->sem_o);
    free(obj);
}