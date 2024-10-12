#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <time.h>

#define MAX_NUM 1028

typedef struct {
    void (*function)(int, int);
    int a, b; 
} Task;

Task queue[MAX_NUM];
int task_count=0;

pthread_mutex_t mut;
pthread_cond_t new_tasks, empty_tasks;

void sum_task(int a, int b){
    printf("Sum of a: %d and b: %d, equals = %d\n", a, b, a+b);
}

void sub_task(int a, int b){
    printf("Sub of a: %d and b: %d, equals = %d\n", a, b, a-b);
}

void mul_task(int a, int b){
    printf("Mul of a: %d and b: %d, equals = %d\n", a, b, a*b);
}

void div_task(int a, int b){
    if(b==0) {
        printf("Cant divide by 0\n");
        return;
    }

    printf("Div of a: %d and b: %d, equals = %d\n", a, b, a/b);
}

void execute_task(Task* task){ 
    
    task->function(task->a, task->b); 
    
}



void* thread_func(void* arg){

    while(1) {
        Task task;

        pthread_mutex_lock(&mut);
        while(task_count<=0){
            pthread_cond_wait(&new_tasks, &mut);
        }
        
        task=queue[0];        
        for(int i=0;i<task_count-1;i++){
            queue[i]=queue[i+1];
        }
        
        task_count--;
        if(task_count==0) pthread_cond_signal(&empty_tasks);
        
        pthread_mutex_unlock(&mut);
        execute_task(&task);
        
    }

    return NULL;
}

void create_tasks(int num){
    printf("Creator thread add %d new task\n", num);
    
    int func_idx;
    for(int i=0;i<num;i++){
        
        
        void* func;
        func_idx=rand()%4;
        if      (func_idx==0) func=sum_task;
        else if (func_idx==1) func=sub_task;
        else if (func_idx==2) func=mul_task;
        else                  func=div_task;
        
        Task task={
            .function=func,
            .a=rand()%1000,
            .b=rand()%1000
        };
        
        queue[task_count++]=task;
    }

    pthread_cond_broadcast(&new_tasks);    
}

void* creator_thread_func(void* arg) {
    while(1){
        pthread_mutex_lock(&mut);        
        if(task_count!=0) pthread_cond_wait(&empty_tasks, &mut);        
        /*EQUIVALENT TO:
        pthread_mutex_unlock(&mut);
        wait for x>5
        pthread_mutex_lock(&mut);
        */

        create_tasks(rand()%MAX_NUM);
        pthread_mutex_unlock(&mut);


        printf("Creator thread sleep for 2 seconds\n");
        sleep(2);
    }
}



int main(int argc, char** argv){
    srand(time(NULL));

    int num_threads=4;
    pthread_t creator_thread;
    pthread_t threads[num_threads];

    pthread_mutex_init(&mut, NULL);
    pthread_cond_init(&new_tasks, NULL); 
    pthread_cond_init(&empty_tasks, NULL);

    // -- create --------------------------------------------------------------
    if(pthread_create(&creator_thread, NULL, &creator_thread_func, NULL)){
        perror("Error while creating a thread");
        exit(EXIT_FAILURE);
    }    
    for(int i=0;i<num_threads;i++){
        if(pthread_create(&threads[i], NULL, &thread_func, NULL)){
            perror("Error while creating a thread");
            exit(EXIT_FAILURE);
        }
    }

    // -- wait ---------------------------------------------------------------
    if(pthread_join(creator_thread, NULL)){
        perror("Error while waiting for the creator thread");
        exit(EXIT_FAILURE);
    }   
    for(int i=0;i<num_threads;i++){
        if(pthread_join(threads[i], NULL)){
            perror("Error while waiting for a thread");
            exit(EXIT_FAILURE);
        }
    }

    pthread_mutex_destroy(&mut);
    pthread_cond_destroy(&new_tasks);
    pthread_cond_destroy(&empty_tasks);

    return 0;
}