#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <vector>
#include <functional>

using namespace std;


class TrafficLight {
private:
    int road_light;
    pthread_mutex_t mut;
public:
    TrafficLight() {
        road_light=0; // 0: A       1: B
        pthread_mutex_init(&mut, NULL);
    }

    void carArrived(
        int carId,                   // ID of the car
        int roadId,                  // ID of the road the car travels on. Can be 1 (road A) or 2 (road B)
        int direction,               // Direction of the car
        function<void()> turnGreen,  // Use turnGreen() to turn light to green on current road
        function<void()> crossCar    // Use crossCar() to make car cross the intersection
    ) {
        pthread_mutex_lock(&mut);
        if(road_light==0&&roadId==2){
            road_light=1;
            turnGreen();
        }
        else if(road_light==1&&roadId==1){
            road_light=0;
            turnGreen();
        }
        
        crossCar();
        pthread_mutex_unlock(&mut);
    }
};