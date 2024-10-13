#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <vector>
#include <functional>

using namespace std;

#define NUM_PHILO 5

class DiningPhilosophers {
private:
    pthread_mutex_t forks[NUM_PHILO];        
public:
    DiningPhilosophers() {
        for(int i=0;i<NUM_PHILO;i++){
            pthread_mutex_init(&forks[i],NULL);
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

        pthread_mutex_lock(&forks[left]);
        pthread_mutex_lock(&forks[right]);
        pickLeftFork();
        pickRightFork();
        eat();
        putLeftFork();
        putRightFork();

        pthread_mutex_unlock(&forks[left]);
        pthread_mutex_unlock(&forks[right]);
        
    }
};