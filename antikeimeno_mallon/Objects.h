#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>

// ================= ENUMS =================

enum class Colour { RED, GREEN, YELLOW };
enum class Direction { NORTH, EAST, SOUTH, WEST };
enum class Speed {
    STOPPED,
    HALF_SPEED,
    FULL_SPEED
};

// ================= POSITION =================

class Position {
    int x;
    int y;
public:
    Position(int x = 0, int y = 0);

    int getX() const;
    int getY() const;
    void set(int nx, int ny);
};

// ================= WORLD OBJECT =================

class WorldObject {
protected:
    int id;
    char glyph;
    Position pos;

public:
    WorldObject(int id, char glyph, int x, int y);
    virtual ~WorldObject();

    int getID() const;
    char getGlyph() const;
    Position getPosition() const;

    virtual void tick();
};

// ================= STATIC OBJECTS =================

class StaticObject : public WorldObject {
public:
    StaticObject(int id, char glyph, int x, int y);
    virtual ~StaticObject();
};

class StationaryVehicle : public StaticObject {
public:
    StationaryVehicle(int id, int x, int y);
    ~StationaryVehicle();
};

class TrafficSign : public StaticObject {
public:
    TrafficSign(int id, int x, int y);
    ~TrafficSign();
};

class TrafficLight : public StaticObject {
    Colour color;
    int ticks;

public:
    TrafficLight(int id, int x, int y);
    ~TrafficLight();

    void tick() override;
    Colour getColor() const;
};

// ================= MOVING OBJECT =================

class MovingObject : public WorldObject {
protected:
    Direction dir;
    Speed speed;

public:
    MovingObject(int id, char glyph, int x, int y, Direction d, Speed s);
    virtual ~MovingObject();

    Speed getSpeed() const;
    Direction getDirection() const;

    void tick() override;
};

// ================= VEHICLES =================

class Car : public MovingObject {
public:
    Car(int id, int x, int y, Direction d);
    ~Car();
};

class Bike : public MovingObject {
public:
    Bike(int id, int x, int y, Direction d);
    ~Bike();
};


const char* dirToStr(Direction d);
const char* speedToStr(Speed s);

#endif // OBJECTS_H
