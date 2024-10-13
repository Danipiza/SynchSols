#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <vector>



using namespace std;

class BoundedBlockingQueue {
private:
    int rear;
    int front;
    int queue_size;
    int count;

    vector<int> queue;

    sem_t sem_full;   
    sem_t sem_empty;  
    sem_t sem_mutex;  

public:
    BoundedBlockingQueue(int capacity) {
        rear=front=count=0;
        queue_size=capacity;

        queue.assign(capacity, -1);

        sem_init(&sem_full, 0, 0);        
        sem_init(&sem_empty, 0, capacity); // capacity empty slots
        sem_init(&sem_mutex, 0, 1);        // binary semaphore
    }

    void enqueue(int element) {
        sem_wait(&sem_empty); // wait an empty slot
        
        sem_wait(&sem_mutex);  

        
        queue[front]=element;
        front=(front+1)%queue_size;
        count++;

        sem_post(&sem_mutex);  
        sem_post(&sem_full);   // signal. an item has been added
    }

    int dequeue() {
        sem_wait(&sem_full);  // wait for an element 

        sem_wait(&sem_mutex);  

        int ret=queue[rear];
        rear=(rear+1)%queue_size;
        count--;

        sem_post(&sem_mutex);  
        sem_post(&sem_empty);   // signal. an item has been removed

        return ret;
    }

    int size() {
        sem_wait(&sem_mutex);  

        int ret=count;      

        sem_post(&sem_mutex);  

        return ret;
    }
};
