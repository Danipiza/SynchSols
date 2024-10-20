using System;
using System.Collections.Generic;
using System.Threading;

public class BoundedBlockingQueue {
    private int rear;
    private int front;
    private int queueSize;
    private int count;

    private List<int> queue;

    private SemaphoreSlim semFull;
    private SemaphoreSlim semEmpty;
    private SemaphoreSlim semMutex;

    public BoundedBlockingQueue(int capacity) {
        
        rear=front=count=0;
        queueSize=capacity;

        queue    =new List<int>(new int[capacity]);

        semFull  =new SemaphoreSlim(0, capacity);
        semEmpty =new SemaphoreSlim(capacity, capacity); 
        semMutex =new SemaphoreSlim(1, 1); 
    }

    public void Enqueue(int element) {
        semEmpty.Wait(); 

        semMutex.Wait();

        queue[front]=element;
        front=(front+1)%queueSize;
        count++;

        semMutex.Release();

        semFull.Release(); 
    }

    public int Dequeue() {
        semFull.Wait(); 

        semMutex.Wait();

        int ret=queue[rear];
        rear=(rear+1)%queueSize;
        count--;

        semMutex.Release();
        semEmpty.Release();

        return ret;
    }

    public int Size() {
        semMutex.Wait();

        int ret=count;

        semMutex.Release();

        return ret;
    }
}