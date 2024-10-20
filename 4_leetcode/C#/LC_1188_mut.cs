using System;
using System.Collections.Generic;
using System.Threading;

public class BoundedBlockingQueue {
    
    private int front;
    private int rear;
    private int queueSize;
    private int capacity;

    private List<int> queue;

    private object lockObj;

    public BoundedBlockingQueue(int capacity) {
        this.capacity=capacity;
        front=rear=queueSize=0;

        queue   =new List<int>(new int[capacity]);       

        lockObj =new object();
    }

    public void Enqueue(int element) {

        lock (lockObj) {
            while (queueSize==capacity) Monitor.Wait(lockObj);

            queue[front]=element;
            front=(front+1)%capacity;
            queueSize++;
            Monitor.PulseAll(lockObj); 
        }
    }

    public int Dequeue() {
        int ret=-1;
        
        lock (lockObj) {
            while (queueSize==0) Monitor.Wait(lockObj);

            ret=queue[rear];
            queue[rear]=-1;
            rear=(rear+1)%capacity;
            queueSize--;
            Monitor.PulseAll(lockObj); 
        }

        return ret;
    }

    public int Size() {
        lock (lockObj) return queueSize;
    }
}
