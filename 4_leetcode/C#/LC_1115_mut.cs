using System;
using System.Threading;

public class FooBar {
    
    private int n;
    private int tick;
    private object mut;

    public FooBar(int n) {
        this.n   =n;
        this.tick=0;

        mut=new object(); 
    }

    public void Foo(Action printFoo) {
        
        for (int i=0;i<n;i++) {
            lock (mut){
                while (tick!=0) Monitor.Wait(mut); // wait
                

                // printFoo() outputs "foo". Do not change or remove this line.
        	    printFoo();
                tick=1;
                Monitor.PulseAll(mut); // broadcast. for all waiting threads
            }
        	
        }
    }

    public void Bar(Action printBar) {
        
        for (int i=0;i<n;i++) {
            lock (mut) {
                while (tick!=1) Monitor.Wait(mut); // wait
                

                // printBar() outputs "bar". Do not change or remove this line.
        	    printBar();
                tick=0;
                Monitor.PulseAll(mut); // broadcast. for all waiting threads
            }
            
        }
    }
}