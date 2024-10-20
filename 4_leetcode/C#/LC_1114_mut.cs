using System;
using System.Threading;

public class Foo {
    
    private int x;
    private object mut;
    private bool cond;

    public Foo() {
        x=1;

        mut =new object(); 
        cond=false; 
    }

    public void First(Action printFirst) {
        lock (mut) {
            while (x!=1) Monitor.Wait(mut); // wait
            

            x++;
            // printFirst() outputs "first". Do not change or remove this line.
            printFirst(); 

            Monitor.PulseAll(mut); // broadcast for all waiting threads
        }
        
        
    }

    public void Second(Action printSecond) {
        lock (mut){
            while (x!=2) Monitor.Wait(mut); // wait
            

            x++;
            // printSecond() outputs "second". Do not change or remove this line.
            printSecond();
            Monitor.PulseAll(mut); // broadcast for all waiting threads
        }
        
    }

    public void Third(Action printThird) {
        lock (mut){
            while (x!=3) Monitor.Wait(mut); // wait
            

            // printThird() outputs "third". Do not change or remove this line.
            printThird();
            Monitor.PulseAll(mut); // broadcast for all waiting threads
        }
        
    }
}