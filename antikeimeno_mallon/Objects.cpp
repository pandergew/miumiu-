#include <iostream>
#include <cstdlib>

#include "Objects.h"

using namespace std;


const char* dirToStr(Direction d) {
    switch (d) {
    case Direction::NORTH: return "NORTH";
    case Direction::SOUTH: return "SOUTH";
    case Direction::EAST:  return "EAST";
    case Direction::WEST:  return "WEST";
    }
    return "";
}

const int speedToInt(Speed s) {
    switch (s) {
    case Speed::STOPPED:     return 0;
    case Speed::HALF_SPEED: return 1;
    case Speed::FULL_SPEED: return 2;
    }
    return -1;
}

// ================= POSITION =================

Position::Position(int x, int y) : x(x), y(y) {}

int Position::getX() const { return x; }
int Position::getY() const { return y; }
void Position::set(int nx, int ny) { x = nx; y = ny; }

// ================= WORLD OBJECT =================

WorldObject::WorldObject(int id, char glyph, int x, int y)
    : id(id), glyph(glyph), pos(x, y) {}

WorldObject::~WorldObject() {}

int WorldObject::getID() const { return id; }
char WorldObject::getGlyph() const { return glyph; }
Position WorldObject::getPosition() const { return pos; }

void WorldObject::tick() {}

// ================= STATIC OBJECT =================

StaticObject::StaticObject(int id, char glyph, int x, int y)
    : WorldObject(id, glyph, x, y) {}

StaticObject::~StaticObject() {}

// ================= STATIONARY VEHICLE =================

StationaryVehicle::StationaryVehicle(int id, int x, int y)
    : StaticObject(id, 'P', x, y) {
    cout << "[+PARKED:" << id << "] at (" << x << "," << y << ")\n";
}

StationaryVehicle::~StationaryVehicle() {
    cout << "[-PARKED:" << id << "] I'm being towed away!\n";
}

// ================= TRAFFIC SIGN =================

TrafficSign::TrafficSign(int id, int x, int y)
    : StaticObject(id, 'S', x, y) {
    cout << "[+SIGN:" << id << "] at (" << x << "," << y << ")\n";
}

TrafficSign::~TrafficSign() {
    cout << "[-SIGN:" << id << "] Removed\n";
}

// ================= TRAFFIC LIGHT =================

TrafficLight::TrafficLight(int id, int x, int y)
    : StaticObject(id, 'L', x, y), ticks(0){
    int r = rand() % 3;
    switch(r) {
        case 0: color = Colour::RED; break;
        case 1: color = Colour::GREEN; break;
        case 2: color = Colour::YELLOW; break;
    }

    string colorStr;
    switch(color) {
        case Colour::RED: colorStr = "RED"; break;
        case Colour::GREEN: colorStr = "GREEN"; break;
        case Colour::YELLOW: colorStr = "YELLOW"; break;
    }

    cout << "[+LIGHT: " << id << "] Initialized at (" << x << "," << y << ") to " << colorStr << endl;
}

TrafficLight::~TrafficLight() {
    cout<< "[-LIGHT: " << id << "] Turning off\n";
}

void TrafficLight::tick() {
    ticks++;

    switch (color) {
    case Colour::RED:
        if (ticks == 4) { color = Colour::GREEN; ticks = 0; }
        break;
    case Colour::GREEN:
        if (ticks == 8) { color = Colour::YELLOW; ticks = 0; }
        break;
    case Colour::YELLOW:
        if (ticks == 2) { color = Colour::RED; ticks = 0; }
        break;
    }
}

Colour TrafficLight::getColor() const {
    return color;
}

// ================= MOVING OBJECT =================

MovingObject::MovingObject(int id, char glyph, int x, int y, Direction d, Speed s)
    : WorldObject(id, glyph, x, y), dir(d), speed(s) {
        cout << "[+VEHICLE: " << id <<"] Created at ("<< x << " , " << y << "), heading " << dirToStr(getDirection()) << " at " << speedToInt(getSpeed()) << " units/ticks\n";
    }

MovingObject::~MovingObject() {}

Speed MovingObject::getSpeed() const { return speed; }
Direction MovingObject::getDirection() const { return dir; }

void MovingObject::tick() {
    int dx = 0, dy = 0;

    int step =
        (speed == Speed::FULL_SPEED) ? 2 :
        (speed == Speed::HALF_SPEED) ? 1 : 0;

    switch (dir) {
    case Direction::NORTH: dy = -step; break;
    case Direction::SOUTH: dy = step; break;
    case Direction::EAST:  dx = step; break;
    case Direction::WEST:  dx = -step; break;
    }

    pos.set(pos.getX() + dx, pos.getY() + dy);
}


// ================= CAR =================

Car::Car(int id, int x, int y, Direction d)
    : MovingObject(id, 'C', x, y, d, Speed::FULL_SPEED) {}

Car::~Car() {
    cout << "[-CAR: " << id << "] Being scrapped...\n";
}

// ================= BIKE =================

Bike::Bike(int id, int x, int y, Direction d)
    : MovingObject(id, 'B', x, y, d, Speed::HALF_SPEED) {}

Bike::~Bike() {
    cout << "[-BIKE: " << id << "] Being locked away...\n";
}

