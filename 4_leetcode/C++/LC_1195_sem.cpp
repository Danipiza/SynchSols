#include <semaphore>
#include <iostream>
#include <thread>

#define NUM_THREADS 4

class FizzBuzz {
private:
    int n;
    int tick;
    bool end;
    std::binary_semaphore sem_mutex;

public:
    FizzBuzz(int n) : 
        n(n), 
        tick(1), 
        end(false), 
        sem_mutex(1) {}
    

    void fizz(function<void()> printFizz) {
        while(true) {
            sem_mutex.acquire();

            while((tick%3!=0 || tick%5==0) && !end) {
                sem_mutex.release();
                sem_mutex.acquire();
            }

            if (tick>n || end) {
                end=true;
                for(int i=0;i<NUM_THREADS;i++) sem_mutex.release();

                break;
            }

            printFizz();
            tick++;
            for(int i=0;i<NUM_THREADS;i++) sem_mutex.release();
        }
    }

        
    void buzz(function<void()> printBuzz) {
        while(true) {
            sem_mutex.acquire();

            while((tick%5!=0 || tick%3==0) && !end) {
                sem_mutex.release();
                sem_mutex.acquire();
            }

            if (tick>n || end) {
                end=true;
                for(int i=0;i<NUM_THREADS;i++) sem_mutex.release();

                break;
            }

            printBuzz();
            tick++;
            for(int i=0;i<NUM_THREADS;i++) sem_mutex.release();
        }
    }
    
    void fizzbuzz(function<void()> printFizzBuzz) {
        while(true) {
            sem_mutex.acquire();

            while((tick%3!=0 || tick%5!=0) && !end) {
                sem_mutex.release();
                sem_mutex.acquire();
            }

            if (tick>n || end) {
                end=true;
                for(int i=0;i<NUM_THREADS;i++) sem_mutex.release();

                break;
            }

            printFizzBuzz();
            tick++;            
            for(int i=0;i<NUM_THREADS;i++) sem_mutex.release();
        }
    }

    void number(function<void(int)> printNumber) {
        while(true) {
            sem_mutex.acquire();

            while((tick%3==0 || tick%5==0) && !end) {
                sem_mutex.release();
                sem_mutex.acquire();
            }

            if (tick>n || end) {
                end=true;
                for(int i=0;i<NUM_THREADS;i++) sem_mutex.release();

                break;
            }

            printNumber(tick);
            tick++;
            for(int i=0;i<NUM_THREADS;i++) sem_mutex.release();
        }
    }
};


        