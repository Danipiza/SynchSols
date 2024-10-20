using System;
using System.Threading;

public class TrafficLight {
    private int roadLight; // 0: A, 1: B
    private object lockObj;

    public TrafficLight(){
        roadLight=0;
        
        lockObj=new object();
    }

    public void CarArrived(int carId, int roadId, int direction, 
        Action turnGreen, Action crossCar) {
            
        lock (lockObj) {
            if (roadLight==0 && roadId==2) {
                roadLight=1;

                turnGreen();
            }
            else if (roadLight==1 && roadId==1) {
                roadLight=0;

                turnGreen();
            }

            crossCar();
        }
    }
}
