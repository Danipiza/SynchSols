#include <semaphore>
#include <iostream>
#include <thread>

class ZeroEvenOdd {
private:
    int n;
    int tick;
    std::binary_semaphore sem_zero;
    std::binary_semaphore sem_odd;
    std::binary_semaphore sem_even;

public:
    ZeroEvenOdd(int n) : 
        n(n), 
        tick(1), 
        sem_zero(1), 
        sem_odd(0), 
        sem_even(0) {}

    void zero(function<void(int)> printNumber) {
        while(true) {
            sem_zero.acquire();

            if(tick>n) {
                sem_odd.release();
                sem_even.release();
                break;
            }

            printNumber(0);

            if(tick%2!=0) sem_odd.release();
            else sem_even.release();
        }
    }

    void even(function<void(int)> printNumber) {
        while(true) {
            sem_even.acquire();

            if(tick>n) break;

            printNumber(tick++);

            sem_zero.release();
        }
    }

    void odd(function<void(int)> printNumber) {
        while(true) {
            sem_odd.acquire();

            if(tick>n) break;

            printNumber(tick++);

            sem_zero.release();
        }
    }
};
