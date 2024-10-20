using System;
using System.Threading;

public class FooBar {
    private int n;

    private SemaphoreSlim semFoo;
    private SemaphoreSlim semBar;

    public FooBar(int n) {
        this.n=n;

        semFoo=new SemaphoreSlim(1, 1);
        semBar=new SemaphoreSlim(0, 1);
    }

    public void Foo(Action printFoo) {
        
        for (int i = 0; i < n; i++) {
            semFoo.Wait();
        	// printFoo() outputs "foo". Do not change or remove this line.
        	printFoo();
            semBar.Release();
        }
    }

    public void Bar(Action printBar) {
        
        for (int i = 0; i < n; i++) {
            semBar.Wait();
            // printBar() outputs "bar". Do not change or remove this line.
        	printBar();
            semFoo.Release();
        }
    }
}