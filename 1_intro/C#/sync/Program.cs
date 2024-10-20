using System;
using System.Threading;

class Program
{
    private static object mut=new object();
    private static int counter;

    static void Main() {
        int num_threads=2,i;
        var threads=new Thread[num_threads];

          
        


        counter=0;
        for(i=0;i<num_threads;i++) {
            threads[i]=new Thread(IncrementCounter_NoSync);
            threads[i].Start();
        }

        for(i=0;i<num_threads;i++) threads[i].Join();
        Console.WriteLine($"Final counter value without sync: {counter}\n");



        counter=0;
        for(i=0;i<num_threads;i++) {
            threads[i]=new Thread(IncrementCounter_Sync);
            threads[i].Start();
        }

        for(i=0;i<num_threads;i++) threads[i].Join();
        Console.WriteLine($"Final counter value with sync: {counter}\n");



    }

    static void IncrementCounter_NoSync() {
        
        for (int i=0;i<10000;i++) {
            counter++;            
        }
    }

    static void IncrementCounter_Sync() {
        
        for (int i=0;i<10000;i++) {

            lock (mut) {
                counter++;
            }
        }
    }
}
