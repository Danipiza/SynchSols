#include <mutex>
#include <condition_variable>
#include <functional>

using namespace std;

class ZeroEvenOdd {
private:
    int n;
    int tick;
    int turn;
    mutex mtx;
    condition_variable cv;

public:
    ZeroEvenOdd(int n) : n(n), tick(1), turn(0) {}

    void zero(function<void(int)> printNumber) {
        unique_lock<mutex> lock(mtx);
        
        while (tick =n) {
            cv.wait(lock, [this] { return turn==0; });
            if (tick>n) break;
            
            printNumber(0);
            
            turn=(tick%2!=0) ? 1:2;
            cv.notify_all();
        }
    }

    void even(function<void(int)> printNumber) {
        unique_lock<mutex> lock(mtx);

        while (tick<=n) {
            cv.wait(lock, [this] { return turn==2; });
            if (tick>n) break;
            
            printNumber(tick++);
            
            turn=0;
            cv.notify_all();
        }
    }

    void odd(function<void(int)> printNumber) {
        unique_lock<mutex> lock(mtx);

        while (tick<=n) {
            cv.wait(lock, [this] { return turn==1; });
            if (tick>n) break;
            
            printNumber(tick++);
            
            turn=0;
            cv.notify_all();
        }
    }
};