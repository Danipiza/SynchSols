using System;
using System.Threading;

public class FizzBuzz {
    private int n;
    private int tick;
    private bool end;
    private object lockObj;

    public FizzBuzz(int n) {
        this.n =n;
        tick   =1;
        end    =false;

        lockObj=new object();
    }

    // printFizz() outputs "fizz".
    public void Fizz(Action printFizz) {
        while (true) {

            lock (lockObj) {
                while ((tick%3!=0 || tick%5==0) && !end) {
                    Monitor.Wait(lockObj);
                }

                if (tick>n || end) {
                    end=true;
                    Monitor.PulseAll(lockObj);

                    return;
                }

                printFizz();
                tick++;
                Monitor.PulseAll(lockObj);
            }
        }
    }

    // printBuzzz() outputs "buzz".
    public void Buzz(Action printBuzz) {
        while (true) {

            lock (lockObj) {
                while ((tick%5!=0 || tick%3==0) && !end) {
                    Monitor.Wait(lockObj);
                }

                if (tick>n || end) {
                    end=true;
                    Monitor.PulseAll(lockObj);
                    return;
                }

                printBuzz();
                tick++;
                Monitor.PulseAll(lockObj);
            }
        }
    }

    // printFizzBuzz() outputs "fizzbuzz".
    public void Fizzbuzz(Action printFizzBuzz) {

        while (true) {
            lock (lockObj) {
                while ((tick%3!=0 || tick%5!=0) && !end) {
                    Monitor.Wait(lockObj);
                }

                if (tick>n || end) {
                    end=true;
                    Monitor.PulseAll(lockObj);
                    return;
                }

                printFizzBuzz();
                tick++;
                Monitor.PulseAll(lockObj);
            }
        }
    }

    // printNumber(x) outputs "x", where x is an integer.
    public void Number(Action<int> printNumber) {
        
        while (true) {
            lock (lockObj) {
                while ((tick%3==0 || tick%5==0) && !end) {
                    Monitor.Wait(lockObj);
                }

                if (tick>n || end) {
                    end=true;
                    Monitor.PulseAll(lockObj);
                    return;
                }

                printNumber(tick);
                tick++;
                Monitor.PulseAll(lockObj);
            }
        }
    }
}