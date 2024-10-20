using System;
using System.Threading;

public class ZeroEvenOdd {
    private int n;

    private int zeroTick;
    private int count;
    private bool end;
    private object mut;
    
    public ZeroEvenOdd(int n) {
        this.n        =n;
        this.zeroTick =1;
        this.count    =1;
        this.end      =false;

        mut=new object();
    }

    // printNumber(x) outputs "x", where x is an integer.
    public void Zero(Action<int> printNumber) {

        while (true) {
            lock (mut) {
                while (zeroTick==0 && !end) Monitor.Wait(mut);

                if (end) return;

                printNumber(0);
                zeroTick=0;
                Monitor.PulseAll(mut);
            }
        }
    }

    public void Even(Action<int> printNumber) {
        
        while (true) {
            lock (mut) {
                while ((zeroTick==1||count%2!=0) && !end) {
                    Monitor.Wait(mut);
                }

                if (end) return;

                printNumber(count);
                count++;
                zeroTick=1;

                if (count>n) {
                    end=true;
                    Monitor.PulseAll(mut);
                    return;
                }

                Monitor.PulseAll(mut);
            }
        }
    }

    public void Odd(Action<int> printNumber) {
        while (true) {
            lock (mut) {
                while ((zeroTick==1||count%2==0) && !end) {
                    Monitor.Wait(mut);
                }

                if (end) return;

                printNumber(count);
                count++;
                zeroTick=1;

                if (count>n) {
                    end=true;
                    Monitor.PulseAll(mut);
                    return;
                }

                Monitor.PulseAll(mut);
            }
        }
    }
}