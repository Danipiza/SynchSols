#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <time.h>

/*
Using a pointer to the array.
Instead of a global variable.

*/


// COMPILE:
// gcc -pthread sum_array_2.c

// EXECUTE
// ./a.out

#define NUM_THREADS 5
#define SIZE_ARRAY 10//000

typedef struct {
    int id;
    int i;
    int j;
    int* array;
} ThreadArgs;



void* func(void* arg){
    ThreadArgs* args=arg;
    int sum=0;

    for(int i=args->i;i<args->j;i++) sum+=args->array[i];    
    printf("Thread %d. Sum: %d\n",args->id,sum);

    int* ret=(int*)malloc(sizeof(int));
    *ret=sum;
    return (void*) ret;
}


int main(int argc, char** argv) {
    
    int array[10]={0,1,2,3,4,5,6,7,8,9};

    pthread_t threads[NUM_THREADS];
    
    int div=SIZE_ARRAY/NUM_THREADS;
    for(int i=0;i<NUM_THREADS;i++){
        ThreadArgs* args=(ThreadArgs*)malloc(sizeof(ThreadArgs));
        args->id=i;
        args->i=div*i;
        args->j=args->i+div;
        args->array=array;

        if(pthread_create(&threads[i], NULL, &func, (void*) args)!=0){
            perror("Error while creating a thread");
            exit(EXIT_FAILURE);
        }
    }

    int sum=0;
    for(int i=0;i<NUM_THREADS;i++){  
        int* ret;      
        if(pthread_join(threads[i],(void*) &ret)){
            perror("Error while waiting for a thread");
            exit(EXIT_FAILURE);
        }
        sum+=*(int*)ret;
        free(ret);
    }

    printf("Final sum: %d\n",sum);

    return 0;
}