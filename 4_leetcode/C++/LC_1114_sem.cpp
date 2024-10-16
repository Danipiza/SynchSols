#include <semaphore>
#include <iostream>
#include <thread>

class Foo {
private:
    counting_semaphore<1> sem1{1};
    counting_semaphore<1> sem2{0};
    counting_semaphore<1> sem3{0};
public:
    Foo() {
        
    }

    void first(function<void()> printFirst) {
        
        // printFirst() outputs "first". Do not change or remove this line.
        printFirst();
        sem2.release();
    }

    void second(function<void()> printSecond) {
        sem2.acquire();
        // printSecond() outputs "second". Do not change or remove this line.
        printSecond();
        sem3.release();
    }

    void third(function<void()> printThird) {
        sem3.acquire();
        // printThird() outputs "third". Do not change or remove this line.
        printThird();
    }
};