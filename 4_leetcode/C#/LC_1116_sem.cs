using System;
using System.Threading;

public class ZeroEvenOdd {
    private int n;

    private int tick;
    private SemaphoreSlim semZero;
    private SemaphoreSlim semOdd;
    private SemaphoreSlim semEven;
    
    public ZeroEvenOdd(int n) {
        this.n=n;
        this.tick=1;

        semZero =new SemaphoreSlim(1, 1);
        semOdd  =new SemaphoreSlim(0, 1);
        semEven =new SemaphoreSlim(0, 1);
    }

    // printNumber(x) outputs "x", where x is an integer.
    public void Zero(Action<int> printNumber) {
        while (true) {
            semZero.Wait();

            if (tick>n) {
                semOdd.Release();
                semEven.Release();
                break;
            }

            printNumber(0);

            if (tick%2!=0) semOdd.Release();
            else semEven.Release();
        }
    }

    public void Even(Action<int> printNumber) {
        while (true) {
            semEven.Wait();

            if (tick>n) break;

            printNumber(tick++);

            semZero.Release();
        }
    }

    public void Odd(Action<int> printNumber) {
        while (true) {
            semOdd.Wait();

            if (tick>n) break;

            printNumber(tick++);

            semZero.Release();
        }
    }
}