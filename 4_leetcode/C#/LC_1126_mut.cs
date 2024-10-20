using System;
using System.Threading;

public class DiningPhilosophers {
    private object[] forks;

    public DiningPhilosophers() {
        forks=new object[5];
        for (int i=0;i<5;i++) forks[i]=new object();
    }

    public void WantsToEat(int philosopher, 
        Action pickLeftFork, Action pickRightFork, 
        Action eat, 
        Action putLeftFork, Action putRightFork) {
            
        int left  =philosopher;
        int right =(philosopher+1)%5;

        // swap left and right for the last philosopher
        if (philosopher==4) (left, right)=(right, left); 

        lock (forks[left]) {
            lock (forks[right]) {
                pickLeftFork();
                pickRightFork();

                eat();
                
                putLeftFork();
                putRightFork();
            }
        }
    }
}
