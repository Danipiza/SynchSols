#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct {
    // User defined data may be declared here.
    int h;
    int o;
    pthread_mutex_t mut;
    pthread_cond_t cond;
} H2O;



H2O* h2oCreate() {
    H2O* obj = (H2O*) malloc(sizeof(H2O));
    
    // Initialize user defined data here.
    obj->h=obj->o=0;
    pthread_mutex_init(&obj->mut, NULL);
    pthread_cond_init(&obj->cond, NULL);
    return obj;
}

void hydrogen(H2O* obj) {
    pthread_mutex_lock(&obj->mut);
    while(obj->h>=2) pthread_cond_wait(&obj->cond, &obj->mut);
    obj->h++;
    if(obj->h==2&&obj->o==1) obj->h=obj->o=0;
    
    // releaseHydrogen() outputs "H". Do not change or remove this line.
    releaseHydrogen();

    pthread_mutex_unlock(&obj->mut);
    pthread_cond_broadcast(&obj->cond);
}

void oxygen(H2O* obj) {
    pthread_mutex_lock(&obj->mut);
    while(obj->o>=1) pthread_cond_wait(&obj->cond, &obj->mut);
    obj->o++;
    if(obj->h==2&&obj->o==1) obj->h=obj->o=0;

    // releaseOxygen() outputs "O". Do not change or remove this line.
    releaseOxygen();
    pthread_mutex_unlock(&obj->mut);
    pthread_cond_broadcast(&obj->cond);
}

void h2oFree(H2O* obj) {
    // User defined data may be cleaned up here.
    pthread_mutex_destroy(&obj->mut);
    pthread_cond_destroy(&obj->cond);
    free(obj);    
}