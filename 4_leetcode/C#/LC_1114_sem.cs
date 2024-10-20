using System;
using System.Threading;

public class Foo {
    
    private SemaphoreSlim[] semaphores;

    public Foo() {
        int[] initVals={ 1, 0, 0 };

        semaphores=new SemaphoreSlim[3];        
        for (int i=0;i<3;i++) {
            semaphores[i]=new SemaphoreSlim(initVals[i]);
        }
    }

    public void First(Action printFirst) {
        
        // printFirst() outputs "first". Do not change or remove this line.
        printFirst();
        semaphores[1].Release();
    }

    public void Second(Action printSecond) {
        semaphores[1].Wait();
        // printSecond() outputs "second". Do not change or remove this line.
        printSecond();
        semaphores[2].Release();
    }

    public void Third(Action printThird) {
        semaphores[2].Wait();
        // printThird() outputs "third". Do not change or remove this line.
        printThird();
    }
}