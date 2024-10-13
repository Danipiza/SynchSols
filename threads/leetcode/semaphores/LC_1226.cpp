#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <vector>
#include <functional>

#define NUM_PHILO 5

class DiningPhilosophers {
private:
    sem_t forks[NUM_PHILO];        
public:
    DiningPhilosophers() {
        for(int i=0;i<NUM_PHILO;i++){
            sem_init(&forks[i],0,1);
        }
    }

    void wantsToEat(int philosopher,
                    function<void()> pickLeftFork,
                    function<void()> pickRightFork,
                    function<void()> eat,
                    function<void()> putLeftFork,
                    function<void()> putRightFork) {
		int left=philosopher;
        int right=(philosopher+1)%NUM_PHILO;

        int tmp;
        if (philosopher==NUM_PHILO-1) {
            tmp=left;
            left=right; right=tmp;
        }

        sem_wait(&forks[left]);
        sem_wait(&forks[right]);
        pickLeftFork();
        pickRightFork();
        eat();
        putLeftFork();
        putRightFork();

        sem_post(&forks[left]);
        sem_post(&forks[right]);
        
    }
};