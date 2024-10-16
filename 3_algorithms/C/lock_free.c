#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <stdatomic.h>

/*
// COMPILE
    gcc -pthread lock_free.c

// EXECUTE
    ./a.out
*/

// stack
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Lock-free stack using an atomic pointer to the head node
typedef struct LockFreeStack {
    _Atomic(Node*) head;
} LockFreeStack;

// Initialize stack
void init_stack(LockFreeStack* stack) { atomic_store(&stack->head, NULL); }

// -----------------------------------------------------------------------------
// (lock-free) Push operation 
void push(LockFreeStack* stack, int value) {
    Node* new_node=(Node*)malloc(sizeof(Node));

    
    new_node->data=value;

    Node* old_head;
    do {
        old_head=atomic_load(&stack->head);
        new_node->next=old_head;
    } while (!atomic_compare_exchange_weak(&stack->head, &old_head, new_node));
}

// (lock-free) Pop operation 
int pop(LockFreeStack* stack) {
    Node* old_head;
    Node* new_head;

    do {
        old_head=atomic_load(&stack->head);
        // Ssack empty
        if (old_head==NULL) return -1;  
        
        new_head=old_head->next;
    } while (!atomic_compare_exchange_weak(&stack->head, &old_head, new_head));

    int value=old_head->data;

    free(old_head);
    return value;
}

// pushing 
void* push_thread(void* arg) {
    LockFreeStack* stack=(LockFreeStack*)arg;

    for (int i=0;i<5;++i) {
        push(stack, i+1);
    //                                   pthread id
        printf("Thread %ld pushed %d\n", pthread_self(), i+1);
    }

    return NULL;
}

// popping
void* pop_thread(void* arg) {
    LockFreeStack* stack=(LockFreeStack*)arg;

    for (int i=0;i<5;i++) {
        int value=pop(stack);

        if (value !=-1) printf("Thread %ld popped %d\n", 
            pthread_self(), value);
        else printf("Thread %ld: Stack is empty\n", 
            pthread_self());        
    }
    return NULL;
}

int main() {
    LockFreeStack stack;
    init_stack(&stack);
    
    int num_push=2, num_pop=2;
    pthread_t push_threads[num_push];
    pthread_t pop_threads[num_pop];

    

    for (int i=0;i<num_push;i++) {        
        if(pthread_create(&push_threads[i], NULL, push_thread, &stack)){
            perror("Error while creating a push thread");
            exit(EXIT_FAILURE);
        }         
    }    
    for (int i=0;i<num_pop;i++) {  
        if(pthread_create(&pop_threads[i], NULL, pop_thread, &stack)){
            perror("Error while creating a pop thread");
            exit(EXIT_FAILURE);
        }         
    }

    
    for (int i=0;i<num_push;i++) {        
        if(pthread_join(push_threads[i], NULL)){
            perror("Error while waiting for a push thread");
            exit(EXIT_FAILURE);
        }         
    }
    for (int i=0;i<num_pop;i++) {  
        if(pthread_join(pop_threads[i], NULL)){
            perror("Error while waiting for a pop thread");
            exit(EXIT_FAILURE);
        }         
    }

    return 0;
}
