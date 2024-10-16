#include <iostream>
#include <vector>

// threads libraries
#include <thread>
#include <mutex>
#include <condition_variable>

// library used for the sleep function
#include <chrono>

using namespace std;

/*
COMPILE:
    g++ -thread -std=c++11 prod_cons.cpp

EXECUTE: 
    ./a.out
*/


struct ThreadArgs {
    int id; // thread ID
    // value that the thread is going to increse/decrease the global variable x.
    int val; 
    int n_consumers;
};

// -- global variables ---------------------------------------------------------
int x=0;
mutex mut;
condition_variable cond;



// -- producer function --------------------------------------------------------
void prod_func(ThreadArgs* args) {
    int val    =args->val;
    int n_cons =args->n_consumers;

    while (1) {

        unique_lock<mutex> lock(mut);
        // pthread_mutex_lock(&mut);

        x+=val;

        cout << "Producer incremented the value of x: " << x << endl;
        
        if (x>5) {           
            if (n_cons==1) cond.notify_one();
            else           cond.notify_all();
            // pthread_cond_signal(&cond);
            // pthread_cond_brodcast(&cond);
        }

        lock.unlock();
        // pthread_mutex_unlock(&mutex);

        this_thread::sleep_for(chrono::seconds(1));
        // sleep(1); // posix function. cant be executed in windows OS.
    }
    
    
    delete args;
    // free(args); // C function to delete dynamic memory
}


// -- consumer function --------------------------------------------------------
void cons_func(ThreadArgs* args) {
    int id  =args->id;
    int val =args->val;

    while (1) {

        unique_lock<mutex> lock(mut);

        while (x<val) {
            printf("Consummer %d. Cant get a piece of the product. Wait...\n",id);
            cond.wait(lock);
            printf("Consummer %d. Awake\n",id);
        }

        x-=val;
        cout << "Consumer " << id << ": Decremented the value of x: " << x << " and takes a break\n";

        cond.notify_all();
        lock.unlock();
        this_thread::sleep_for(chrono::seconds(1));
    }

    delete args;
}

int main() {
    int n_cons=3; // number of consumers

    thread prod;
    vector<thread> consumers;
    
    // -- init -----------------------------------------------------------------
    ThreadArgs* args = new ThreadArgs();
    args->id  =0;
    args->val =8;
    args->n_consumers=n_cons;
    
    // producer thread
    prod=thread(prod_func, args);
    // pthread_create(&prod, NULL, &prod_func, NULL);

    // consumer threads
    for (int i=0;i<n_cons;i++) {
        ThreadArgs* cons_args=new ThreadArgs();
        cons_args->id  =i;
        cons_args->val =5;

        consumers.push_back(thread(cons_func, cons_args));        
    }
    
    // -- wait for the threads -------------------------------------------------
    // not strictly necessary. the thread functions are in infinites loops       
    prod.join();
    // pthread_join(&prod, NULL);
    for (auto& cons: consumers) cons.join();

    
    return 0;
}
