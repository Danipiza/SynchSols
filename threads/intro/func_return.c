#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <time.h>


#define MAX_MSG_LEN 80

typedef struct{
    int val;
    char* msg[2];
} ThreadArgs;

void* struct_var_func(void* args){
    ThreadArgs* ret=(ThreadArgs*)malloc(sizeof(ThreadArgs));
    
    int val=(rand()%100)+1;
    ret->val=val;
    
    ret->msg[0]=(char*)malloc(MAX_MSG_LEN*sizeof(char));
    ret->msg[1]=(char*)malloc(MAX_MSG_LEN*sizeof(char));
    sprintf(ret->msg[0],"Random value generated:");          
    sprintf(ret->msg[1],"Memory: %p\n", ret);
    
    return (void*) ret;
}


void* one_var_func(void* args) {
    int* ret=(int*) malloc(sizeof(int));

    int val=(rand()%100)+1;    
    *ret=val;
    
    printf("Random value generated: %d\tMemory: %p\n", 
            *ret, ret);    
    return (void*) ret;
}

void one_var() {
    srand(time(NULL));
    int* ret_val;
    
    pthread_t t;
    if(pthread_create(&t, NULL, one_var_func, NULL)!=0) {
        perror("Error while creating the thread");
        exit(EXIT_FAILURE);
    }
    
    
    
    if(pthread_join(t,(void**)&ret_val)){
        perror("Error while waiting for the thread");
        exit(EXIT_FAILURE);
    } 

    printf("Return value: %d\t\tMemory: %p\n", 
            *ret_val, ret_val);   

    free(ret_val);
}

void struct_var() {
    srand(time(NULL));
    ThreadArgs* ret_val;
    
    pthread_t t;
    if(pthread_create(&t, NULL, struct_var_func, NULL)!=0) {
        perror("Error while creating the thread");
        exit(EXIT_FAILURE);
    }
    
    
    
    if(pthread_join(t,(void**)&ret_val)){
        perror("Error while waiting for the thread");
        exit(EXIT_FAILURE);
    }
    
    //"Return value: %d\t\tMemory: %p\n", 
    printf("%s: %d\t%s",ret_val->msg[0], ret_val->val, ret_val->msg[1]);


    free(ret_val);
}

int main(int argc, char** argv) {
    //one_var();
    struct_var();


    return 0;
}