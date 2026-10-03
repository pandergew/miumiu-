#include "SelfDrivingCar.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

SelfDrivingCar::SelfDrivingCar(int id, int x1, int y1, int x2, int y2)
    : WorldObject(id, glyph, x1, y1), gpsTarget(x2, y2),
      dir(Direction::NORTH), speed(Speed::STOPPED) {
    cout << "[+Car:" << id << "] Initialized at "
         << x1 << "," << y1 << ") facing " << dirToStr(getDirection()) << " -No driver's licence required!\n";
}

SelfDrivingCar::~SelfDrivingCar() {
    cout << "Our journey is complete\n";
}

void SelfDrivingCar::accelerate() {
    if(speed == Speed::STOPPED) speed = Speed::HALF_SPEED;
    else if(speed == Speed::HALF_SPEED) speed = Speed::FULL_SPEED;
}

void SelfDrivingCar::decelerate() {
    if(speed == Speed::FULL_SPEED) speed = Speed::HALF_SPEED;
    else if(speed == Speed::HALF_SPEED) speed = Speed::STOPPED;
}

void SelfDrivingCar::turn(Direction newDir) {
    dir = newDir;
}

// απλή tick: μετακινεί το όχημα ανάλογα με speed & dir
void SelfDrivingCar::tick() {
    int step = (speed == Speed::FULL_SPEED) ? 2 :
               (speed == Speed::HALF_SPEED) ? 1 : 0;

    int dx = 0, dy = 0;
    switch(dir) {
        case Direction::NORTH: dy = -step; break;
        case Direction::SOUTH: dy = step; break;
        case Direction::EAST: dx = step; break;
        case Direction::WEST: dx = -step; break;
    }

    pos.set(pos.getX() + dx, pos.getY() + dy);
}


//αξιολύπητη προσπάθεια για sencors 
struct SensorReading {
    string type;
    int distance;
    int x, y; // position
    string objectId;
    double confidence;
    //(auto to apokatw bgainei apo to tick to speed:)
    int speed;
    int direction;
    string signText;
    char trafficLight;
};

class Sensor {
    protected:
        int range;
        int view_field;
        int accuracy;
    public:
        virtual vector<SensorReading> sensorreading () = 0; 
        virtual ~Sensor() {}
};

vector<SensorReading> LidarSensor:: sense 

class LidarSensor : public Sensor {
    public:
        LidarSensor() {
            range = 9;
            view_field = 9;
            accuracy = 0.97;
        }

        vector<SensorReading> sensorreading () override;
};
