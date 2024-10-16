#include <semaphore>
#include <iostream>
#include <thread>


class FooBar {
private:
    int n;
    counting_semaphore<1> sem_foo{1};
    counting_semaphore<1> sem_bar{0};

public:
    FooBar(int n) {
        this->n = n;
    }

    void foo(function<void()> printFoo) {
        
        for (int i = 0; i < n; i++) {
            sem_foo.acquire();
        	// printFoo() outputs "foo". Do not change or remove this line.
        	printFoo();
            sem_bar.release();
        }
    }

    void bar(function<void()> printBar) {
        
        for (int i = 0; i < n; i++) {
            sem_bar.acquire();
        	// printBar() outputs "bar". Do not change or remove this line.
        	printBar();
            sem_foo.release();
        }
    }
};