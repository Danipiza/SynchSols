#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// COMPILE:
// gcc -pthread intro.c

// EXECUTE
// ./a.out

#define NUM_THREADS 5
#define MAX_MSG_LEN 80

// while creating a thread, the thread use a function
void* func_one(void *arg){
    long thread_id=(long)arg;

    printf("Hello from thread: %d\n", thread_id);
    sleep(2);
    printf("Ending thread: %d\n", thread_id);
    //return NULL;
}

typedef struct {
    long id;
    char msg[MAX_MSG_LEN];
} ThreadArgs;

void* func_struct(void *args){

    ThreadArgs* struct_args=(ThreadArgs*)args;
    // cant free the variable. it is a pointer
    // free(args);  

    printf("%s", struct_args->msg);
    sleep(2);
    printf("Ending thread: %d\n", struct_args->id);
} 

void one_variable_threads(){
    long i;
    char msg[80];

    // threads
    pthread_t threads[NUM_THREADS];

    /* init. 
    int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg);
    
    ARGS:
        pthread_t *thread: thread variable.
        const pthread_attr_t *attr: thread attributes.
        void *(*start_routine) (void *): thread function.
        void *arg: thread function variables.
    */
    for(i=0;i<NUM_THREADS;i++){

        if(pthread_create(&threads[i], NULL, func_one, (void *) i) != 0){
            sprintf(msg, "Error while creating ID:%d thread\n", i);
            perror(msg);
            exit(EXIT_FAILURE);
        }
    }


    /* wait for the thread to finalizes
    int pthread_join(pthread_t thread, void **retval);

    ARGS:
        thread variable
        return value
    */
    for(i=0; i<NUM_THREADS; i++){
        if(pthread_join(threads[i], NULL) != 0){
            sprintf(msg, "Error while waiting for ID:%d thread\n", i);
            perror(msg);            
            exit(EXIT_FAILURE);
        }
    }

    
}


void more_variables_threads(){
    long i;
    char msg[MAX_MSG_LEN];

    // threads
    pthread_t threads[NUM_THREADS];

    /* init. 
    int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg);
    
    ARGS:
        pthread_t *thread: thread variable.
        const pthread_attr_t *attr: thread attributes.
        void *(*start_routine) (void *): thread function.
        void *arg: thread function variables.
    */
    for(i=0;i<NUM_THREADS;i++){
        
        
        ThreadArgs* args=malloc(sizeof(ThreadArgs));
        if (args==NULL) {
            perror("Failed to allocate memory for thread arguments");
            exit(EXIT_FAILURE);
        }
        args->id=i;
        sprintf(args->msg, "Hello from thread: %ld\n", i);
        
        

        if(pthread_create(&threads[i], NULL, func_struct, args) != 0){
            sprintf(msg, "Error while creating ID:%d thread\n", i);
            perror(msg);
            exit(EXIT_FAILURE);
        }
    }

    /* wait for the thread to finalizes
    int pthread_join(pthread_t thread, void **retval);

    ARGS:
        thread variable
        return value
    */
    for(i=0; i<NUM_THREADS; i++){
        if(pthread_join(threads[i], NULL) != 0){
            sprintf(msg, "Error while waiting for ID:%d thread\n", i);
            perror(msg);            
            exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char** argv) {
    //one_variable_threads();
    more_variables_threads();

    return 0;
}