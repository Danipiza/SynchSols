#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <vector>

using namespace std;

class BoundedBlockingQueue {
private:
    vector<int> queue;   
    int front; 
    int rear;
    int queue_size;

    int capacity;
    
    pthread_mutex_t mut;
    pthread_cond_t full, empty;

public:
    BoundedBlockingQueue(int capacity) {
        queue.assign(capacity, -1);
        front=rear=queue_size=0;
        
        this->capacity=capacity;       
        

        pthread_mutex_init(&mut, NULL);
        pthread_cond_init(&full, NULL);
        pthread_cond_init(&empty, NULL);
    }
    
    void enqueue(int element) {
        // full -> block
        pthread_mutex_lock(&mut);
        while(queue_size==capacity) pthread_cond_wait(&full, &mut); 

        queue[front]=element;
        front=(front+1)%capacity;
        queue_size++;      
        pthread_mutex_unlock(&mut);
        pthread_cond_broadcast(&empty);
    }
    
    int dequeue() {
        // empty -> block
        pthread_mutex_lock(&mut);
        while(queue_size==0) pthread_cond_wait(&empty, &mut);

        int ret=queue[rear];
        queue[rear]=-1;
        rear=(rear+1)%capacity;
        queue_size--;

        pthread_mutex_unlock(&mut);
        pthread_cond_broadcast(&full);
        return ret;
    }
    
    int size() { 
        pthread_mutex_lock(&mut);
        int ret=queue_size;
        pthread_mutex_unlock(&mut);
        return ret;
    }
};