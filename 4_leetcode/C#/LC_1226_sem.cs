using System;
using System.Threading;

public class DiningPhilosophers {

    private const int NUM_PHILO=5;
    private SemaphoreSlim[] forks;

    public DiningPhilosophers() {

        forks=new SemaphoreSlim[NUM_PHILO];
        for (int i=0;i<NUM_PHILO;i++) {
            forks[i]=new SemaphoreSlim(1, 1);
        }
    }

    public void WantsToEat(int philosopher,
            Action pickLeftFork, Action pickRightFork,
            Action eat,
            Action putLeftFork, Action putRightFork) {

        int left  =philosopher;
        int right =(philosopher+1)%NUM_PHILO;

        if (philosopher==NUM_PHILO-1) {
            int tmp=left;
            
            left   =right;
            right  =tmp;
        }

        forks[left].Wait();
        forks[right].Wait();

        pickLeftFork();
        pickRightFork();

        eat();

        putLeftFork();
        putRightFork();

        forks[left].Release();
        forks[right].Release();
    }    
}