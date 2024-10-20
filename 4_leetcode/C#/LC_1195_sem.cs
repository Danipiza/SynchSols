using System;
using System.Threading;

public class FizzBuzz {
    private int n;

    private const int NUM_THREADS=4;
    private int tick;
    private bool end;
    private SemaphoreSlim semMutex;

    public FizzBuzz(int n) {      
        this.n =n;
        tick   =1;
        end    =false;

        semMutex=new SemaphoreSlim(1, 1);
    }

    // printFizz() outputs "fizz".
    public void Fizz(Action printFizz) {
        while (true) {
            semMutex.Wait();

            try {
                while ((tick%3!=0 || tick%5==0) && !end) {
                    semMutex.Release();
                    semMutex.Wait();
                }

                if (tick>n || end) {
                    end=true;
                    break;
                }

                printFizz();
                tick++;
            }
            finally {
                semMutex.Release();
            }
        }
    }

    // printBuzzz() outputs "buzz".
    public void Buzz(Action printBuzz) {
        
        while (true) {
            semMutex.Wait();

            try {
                while ((tick%5!=0 || tick%3==0) && !end) {
                    semMutex.Release();
                    semMutex.Wait();
                }

                if (tick>n || end) {
                    end=true;
                    break;
                }

                printBuzz();
                tick++;
            }
            finally {
                semMutex.Release();
            }
        }
    }

    // printFizzBuzz() outputs "fizzbuzz".
    public void Fizzbuzz(Action printFizzBuzz) {
        while (true) {
            semMutex.Wait();

            try {
                while ((tick%3!=0 || tick%5!=0) && !end) {
                    semMutex.Release();
                    semMutex.Wait();
                }

                if (tick>n || end) {
                    end=true;
                    break;
                }

                printFizzBuzz();
                tick++;
            }
            finally {
                semMutex.Release();
            }
        }
    }

    // printNumber(x) outputs "x", where x is an integer.
    public void Number(Action<int> printNumber) {
        while (true) {
            semMutex.Wait();

            try {
                while ((tick%3==0 || tick%5==0) && !end) {
                    semMutex.Release();
                    semMutex.Wait();
                }

                if (tick>n || end) {
                    end=true;
                    break;
                }

                printNumber(tick);
                tick++;
            }
            finally {
                semMutex.Release();
            }
        }
    }
}