#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// COMPILE:
// gcc -pthread intro.c

// EXECUTE
// ./a.out

#define NUM_THREADS 3

pthread_mutex_t mut[3];
int vals[3]={10,10,100};

void* simple_func(void *arg){
    long idx=(long)arg;
    usleep(10);

    if(pthread_mutex_trylock(&mut[0])==0){
        printf("Thread %d, GOT the lock\n",idx);
        usleep(10);
        pthread_mutex_unlock(&mut[0]);
    }
    else {  
        printf("Thread %d, did not get the lock\n",idx);
    }
}

void simple_execution(){
    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&mut[0], NULL);

    for(long i=0;i<NUM_THREADS;i++){
        if(pthread_create(&threads[i], NULL, &simple_func, (void *) i) != 0){            
            perror("Error while creating a thread");
            exit(EXIT_FAILURE);
        }
    }

    for(long i=0; i<NUM_THREADS; i++){
        if(pthread_join(threads[i], NULL) != 0){            
            perror("Error while waiting for a thread\n");
            exit(EXIT_FAILURE);
        }
    }

    pthread_mutex_destroy(&mut[0]);
}   

void* func(void* arg){  
    long idx=(long) arg;

    while(1){
        for(int i=0;i<3;i++){
            if(pthread_mutex_trylock(&mut[i])==0) {           

                if(vals[i]>=5){
                    vals[i]-=5;
                    printf("Thread %d, get values from vals[%d]\n",idx,i);
                }
                else printf("Thread %d, DONT get values\n",idx);

                pthread_mutex_unlock(&mut[i]);
                return NULL;
            }
        }        
    }
}

void execution(){
    pthread_t thread[10];
    for(int i=0;i<3;i++) pthread_mutex_init(&mut[i], NULL);

    for(long i=0;i<10;i++){
        if(pthread_create(&thread[i], NULL, &func, (void*) i)){
            perror("Error while creating a thread");
            exit(EXIT_FAILURE);
        }
    }

    for(int i=0;i<10;i++){
        if(pthread_join(thread[i], NULL)){
            perror("Error while waiting for a thread");
            exit(EXIT_FAILURE);
        }
    }
    
    for(int i=0;i<3;i++) pthread_mutex_destroy(&mut[i]);
}


int main(int argc, char** argv) {
    //simple_execution();
    execution();

    return 0;
}