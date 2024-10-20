using System;
using System.Threading;

public class TrafficLight {
    
    private int roadLight;
    private SemaphoreSlim semMutex;

    public TrafficLight() {
        roadLight =0; // 0: A       1: B
        semMutex  =new SemaphoreSlim(1, 1); // binary semaphore
    }

    public void CarArrived(
        int carId,                   // ID of the car
        int roadId,                  // ID of the road the car travels on. Can be 1 (road A) or 2 (road B)
        int direction,               // Direction of the car
        Action turnGreen,            // Use turnGreen() to turn light to green on current road
        Action crossCar              // Use crossCar() to make car cross the intersection
    )
    {
        semMutex.Wait();
        try {
        
            if (roadLight==0 && roadId==2) {
                roadLight=1;
                turnGreen();
            }
            else if (roadLight==1 && roadId==1) {
                roadLight=0;
                turnGreen();
            }
            
            crossCar();
            
        finally {
            semMutex.Release();
        }
    
        
        
    }
}