using System;
using System.Threading;

class Program {

    static void func(int id) {
        Console.WriteLine($"Hello from thread {id}");
        
        Thread.Sleep(id * 1000); // sleeps for "id" seconds.

        Console.WriteLine($"Thread {id} finished after sleeping for {id} seconds");
    }

    static void Main(string[] args) {
        int i;
        Console.WriteLine("Main thread starting...");

        
        // -- create the threads  ----------------------------------------------        
        var threads=new Thread[5];
        for (i=0;i<5;i++) {
            int threadId = i + 1; // Thread IDs will be 1-5

            // used for functions without parameters
            // var thread = new Thread(new ThreadStart(func));
            threads[i] = new Thread(() => func(threadId));
            threads[i].Start();
        }

        Console.WriteLine("Main thread waiting for all threads to finish...");

        
        // -- wait for the created threads -------------------------------------        
        foreach (Thread thread in threads) thread.Join();

        Console.WriteLine("\nAll threads have finished. Main thread ending.");
    }

    
}