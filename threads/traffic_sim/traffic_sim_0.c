#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include <time.h>

#define MAX_CAR 10

int road;
int road_x[MAX_CAR], road_y[MAX_CAR];
int count_x, count_y;
int tick;

pthread_mutex_t mutex;
pthread_cond_t change_color;

/* MULTIPLE THREADS. but there is only one
pthread_mutex_t FA_mutex;
int ticket_x, ticket_y;


int fetch_and_add(int* ticket, int idx){

}*/

void create_cars(){
    int num;

    if(count_x!=10){
        num=rand()%(10-count_x);
        printf("New cars arrived at ROAD_X: %d\n",num);
        for(int i=0;i<num;i++){
            road_x[count_x]=road_x[count_x-1]+rand()%3;
            count_x++;
        }
    }

    
    if(count_y!=10){
        num=rand()%(10-count_y);
        
        printf("New cars arrived at ROAD_Y: %d\n",num);
        for(int i=0;i<num;i++){
            road_x[count_y]=road_x[count_y-1]+rand()%3;
            count_y++;
        }
    }
}


void* func_x(void* arg){
    int n=0;

    while(1) {
        //printf("LLEGAX\n");
        pthread_mutex_lock(&mutex);
        while(road!=0) pthread_cond_wait(&change_color, &mutex);            

        tick++;
        if(count_x>0&&road_x[0]<=tick){
            printf("ROAD_X: Car number %d pass the intersection\tqueue: %d\n", ++n,count_x-1);
            for(int i=0;i<count_x-1;i++) road_x[i]=road_x[i+1];
            count_x--;
        }


        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
}

void* func_y(void* arg){
    
    int n=0;

    while(1) {
        pthread_mutex_lock(&mutex);
        while(road!=1) pthread_cond_wait(&change_color, &mutex);            

        tick++;
        if(count_y>0&&road_y[0]<=tick){
            printf("ROAD_Y: Car number %d pass the intersection\tqueue: %d\n", ++n,count_y-1);
            for(int i=0;i<count_y-1;i++) road_y[i]=road_y[i+1];
            count_y--;
        }


        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
}

int main(int argc, char** argv){
    // road initial values. one car in each road
    road_x[0]=1;
    road_y[0]=1;
    count_x=count_y=1;


    pthread_t thread_x, thread_y;
    pthread_attr_t detached_attr;

    pthread_attr_init(&detached_attr);
    pthread_attr_setdetachstate(&detached_attr, PTHREAD_CREATE_DETACHED);
    
    
    if(pthread_create(&thread_x, NULL, &func_x, NULL) ||
       pthread_create(&thread_y, NULL, &func_y, NULL)){
        perror("Error while creating a thread");
        exit(EXIT_FAILURE);
    }
    pthread_attr_destroy(&detached_attr);

    // main creating new cars in each direction and 
    // changing the semaphore's lights of the street.
    
    road=-1;
    tick=0;

    char* lights[2]={"ROAD_X: GREEN\tROAD_Y: RED",
                     "ROAD_X: RED\tROAD_Y: GREEN"};
    
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&change_color, NULL);
    
    printf("LLEGA\n");
    while(1){                
        road=(road+1)%2;
        create_cars();
        
        pthread_cond_broadcast(&change_color);
        

        printf("%s\n",lights[road]);

        for(int i=0;i<5;i++){ tick++; sleep(1); }        
    }

    

    return 0;
}
