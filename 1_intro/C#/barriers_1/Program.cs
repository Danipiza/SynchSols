using System;
using System.Threading;

class Program
{
    static Barrier barrier;
    static int num_threads=5;

    static void func(int id) {
        int id_=0;
        if(!string.IsNullOrEmpty(Thread.CurrentThread.Name)){
            int.Parse(Thread.CurrentThread.Name);
        }
        
        Console.WriteLine($"Thread {id}. Sleeping...");        
        
        Thread.Sleep(id*1000); 
        
        Console.WriteLine($"Thread {id}. Waiting at the barrier...");
        barrier.SignalAndWait();
        
        Console.WriteLine($"Thread {id}. Finished.");
    }

    static void Main(string[] args) {
        

        // barrier with the number of threads created and the main thread
        barrier=new Barrier(num_threads+1);

        
        Thread[] threads=new Thread[num_threads];        
        for (int i=0;i<num_threads;i++) {            
            threads[i]=new Thread(() => func(i+1));
            threads[i].Name = $"{i+1}";

            threads[i].Start();
        }

        // main thread waits at the barrier
        Console.WriteLine("Main thread waiting at the barrier...");
        barrier.SignalAndWait();
        

        // wait for all threads to finish
        for (int i=0;i<num_threads;i++) threads[i].Join();

        Console.WriteLine("\nAll threads have finished.");
    }

    
}