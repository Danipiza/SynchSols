#include <mutex>
#include <condition_variable>
#include <functional>

using namespace std;

class Foo {
private:
    mutex mtx;
    condition_variable cv;
    int count;

public:
    Foo() : count(0) {}

    void first(function<void()> printFirst) {
        unique_lock<mutex> lock(mtx);
        
        // printFirst() outputs "first". Do not change or remove this line.
        printFirst();
        
        count++;
        cv.notify_all();
    }

    void second(function<void()> printSecond) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [this]() { return count>=1; });
        //while(obj->x!=1) pthread_cond_wait(&obj->cond, &obj->mut);
        
        // printSecond() outputs "second". Do not change or remove this line.
        printSecond();
        
        count++;
        cv.notify_all();
    }

    void third(function<void()> printThird) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [this]() { return count>=2; });
        
        // printThird() outputs "third". Do not change or remove this line.
        printThird();
        
        count=0; 
    }
};