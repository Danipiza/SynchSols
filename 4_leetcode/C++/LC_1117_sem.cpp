#include <semaphore>
#include <iostream>
#include <thread>

class H2O {
private:
    int h;
    int o;

    counting_semaphore<1> sem_global{1};
    counting_semaphore<1> sem_h{2};
    counting_semaphore<1> sem_o{1};
public:
    H2O() {
        h=2;
        o=1;
    }

    void reset_values(){
        if(h==0&&o==0){
            h=2;
            o=1;
            sem_h.release();
            sem_h.release();
            sem_o.release();            
        }
    }

    void hydrogen(function<void()> releaseHydrogen) {
        sem_h.acquire();

        // releaseHydrogen() outputs "H". Do not change or remove this line.
        releaseHydrogen();

        sem_global.acquire();
        h--;
        reset_values();
        sem_global.release();
    }

    void oxygen(function<void()> releaseOxygen) {
        sem_o.acquire();

        // releaseOxygen() outputs "O". Do not change or remove this line.
        releaseOxygen();

        sem_global.acquire();
        o--;
        reset_values();
        sem_global.release();
    }
};