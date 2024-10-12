#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int x=0;
pthread_mutex_t mut;
pthread_cond_t cond;

typedef struct {
    int id;
    int val;
    int n_consummers;
} ThreadArgs;

// doesnt free because is an infinite loop
void* prod_func(void* arg){
    ThreadArgs* args=arg;
    
    int val=args->val;
    int n_cons=args->n_consummers;
        

    while(1){
        pthread_mutex_lock(&mut);
        
        x+=val;

        printf("Producer increment the value of x: %d\n", x);               
        
        pthread_mutex_unlock(&mut);

        if(x>5) {
            if(n_cons==1) pthread_cond_signal(&cond);
            else pthread_cond_broadcast(&cond);
        }
        sleep(1);
    }
}

// doesnt free because is an infinite loop
void* cons_func(void* arg){
    ThreadArgs* args=arg;
    
    int id=args->id;
    int val=args->val;
    

    while(1){
        pthread_mutex_lock(&mut);
        while(x<val){
            printf("Consummer %d. Cant get a piece of the product. Wait...\n",id);
            pthread_cond_wait(&cond, &mut);
            printf("Consummer %d. Awake\n",id);
            /*EQUIVALENT TO:
            pthread_mutex_unlock(&mut);
            wait for x>5
            pthread_mutex_lock(&mut);
            */
        }

        x-=val;
        printf("Consummer %d. Decrement the value of x: %d. And takes a break\n", id, x);

        pthread_mutex_unlock(&mut);
        pthread_cond_broadcast(&cond);
        usleep(500);
    }
}


int main(int argc, char** argv){    
    int n_cons=3; 

    pthread_t prod, cons[n_cons];
    pthread_mutex_init(&mut, NULL);
    pthread_cond_init(&cond, NULL);

      

    ThreadArgs* args=(ThreadArgs*)malloc(sizeof(ThreadArgs));
    args->id=0;
    args->val=8;
    args->n_consummers=n_cons;
    

    if(pthread_create(&prod, NULL, &prod_func, (void*) args)){
        perror("Error while creating the producer thread");
        exit(EXIT_FAILURE);
    }
    for(int i=0;i<n_cons;i++){
        args=(ThreadArgs*)malloc(sizeof(ThreadArgs));
        args->id=i;
        args->val=5;
        //args->n_consummers=n_cons;
        if(pthread_create(&cons[i], NULL, &cons_func, (void*) args)){
            perror("Error while creating a consummer thread");
            exit(EXIT_FAILURE);
        }
    }

    
    // -- not necessary. is an infinite loop. ----------------------------------
    if(pthread_join(prod, NULL)){
        perror("Error while waiting for producer thread");
        exit(EXIT_FAILURE);
    }
    for(int i=0;i<n_cons;i++){
        if(pthread_join(cons[i], NULL)){
            perror("Error while waiting for a consummer thread");
            exit(EXIT_FAILURE);
        }
    }

    pthread_mutex_destroy(&mut);
    pthread_cond_destroy(&cond);

    return 0;
}