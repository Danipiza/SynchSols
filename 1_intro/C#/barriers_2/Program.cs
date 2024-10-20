using System;
using System.Threading;
using System.Threading.Tasks;

class Program {
    
    static void Main(string[] args) {
        const int num_threads =5;
        const int num_phases  =3;

        var barrier = new Barrier(num_threads, (b) => {
            Console.WriteLine($"Phase {b.CurrentPhaseNumber} completed. Starting next phase...");
        });

        

        Parallel.For(0, num_threads, i => {
            
            for (int j=0;j<num_phases;j++) {
                
                Thread.Sleep(1000*(i+1)); // simulate code

                Console.WriteLine($"Thread {i}. completed work for phase {j}");

                // wait for all threads
                barrier.SignalAndWait();

                Console.WriteLine($"Thread {i}. Starting next phase...");
            }
        });

        Console.WriteLine("\nAll phases completed.");
    }
}