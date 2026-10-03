#ifndef SELFDRIVINGCAR_H
#define SELFDRIVINGCAR_H

#include "Objects.h"
#include <vector>

class SelfDrivingCar : public WorldObject {
private:
    Position gpsTarget;   
    Direction dir;
    Speed speed;

public:
    SelfDrivingCar(int id, int x1, int y1, int x2, int y2);
    ~SelfDrivingCar();

    // movement
    void accelerate();
    void decelerate();
    void turn(Direction newDir);

    // GPS
    Position getPosition() const { return pos; }
    Position getTarget() const { return gpsTarget; }
    Direction getDirection() const { return dir; }
    Speed getSpeed() const { return speed; }

    void syncNavigationSystem();
    void collectSensorData();
    void executeMovement();

    void tick();  // ενημέρωση κίνησης ανά tick
};

#endif
