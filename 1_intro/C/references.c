#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


#define NUM_THREADS 4

int vals[NUM_THREADS]= {1, 10, 100, 1000};


void* func1(void* args) {
    sleep(1);
    int idx=*(int*)args;
    printf("Thread %ld: Value: %d\n", idx, vals[idx]); //)Value: %d\n", idx, vals[idx]);
}

void incorrect_use(){
    pthread_t threads[NUM_THREADS];

    for(int i=0;i<NUM_THREADS;i++){
        // pass the references
        if(pthread_create(&threads[i], NULL, &func1, &i)!=0){
            perror("Error while creating a thread");
            exit(EXIT_FAILURE);
        }
    }
    
    for(int i=0;i<NUM_THREADS;i++){
        if(pthread_join(threads[i], NULL)!=0){
            perror("Error while waiting for a thread");
            exit(EXIT_FAILURE);
        }
    }
}


void* func2(void* args) {
    sleep(1);
    int idx=*(int*)args;
    printf("Thread %ld: Value: %d\n", idx, vals[idx]); //)Value: %d\n", idx, vals[idx]);
    free(args);
}

void correct_use(){
    pthread_t threads[NUM_THREADS];

    for(int i=0;i<NUM_THREADS;i++){
        int* idx=malloc(sizeof(int));
        *idx=i;
        // pass the references
        if(pthread_create(&threads[i], NULL, &func2, idx)!=0){
            perror("Error while creating a thread");
            exit(EXIT_FAILURE);
        }
    }
    
    for(int i=0;i<NUM_THREADS;i++){
        if(pthread_join(threads[i], NULL)!=0){
            perror("Error while waiting for a thread");
            exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char** argv) {    
    //incorrect_use();
    correct_use();

    return 0;
}