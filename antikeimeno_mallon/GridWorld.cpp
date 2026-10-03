#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#include "GridWorld.h"
#include "Objects.h"
#include "SelfDrivingCar.h"

using namespace std;

// Helper to generate random position
Position GridWorld::randomPos() {
    int x = rand() % dimX;
    int y = rand() % dimY;
    return Position(x, y);
}

// ----- Default constructor -----
GridWorld::GridWorld() {
    seed = time(0);
    dimX = 40; dimY = 40;
    numMovingCars = 3;
    numMovingBikes = 4;
    numParkedCars = 5;
    numStopSigns = 2;
    numTrafficLights = 2;
    simulationTicks = 100;
    minConfidenceThreshold = 40;

    id = seed % 10000;
    srand(seed);

    cout << "[+WORLD: " << id << "] Reticulating splines - Hello, world!" << endl;

    generateWorld();
}

// ----- Parameterized constructor -----
GridWorld::GridWorld(int seed, int dimX, int dimY,
                     int numMovingCars, int numMovingBikes, int numParkedCars,
                     int numStopSigns, int numTrafficLights,
                     int simulationTicks, int minConfidenceThreshold)
    : seed(seed), dimX(dimX), dimY(dimY),
      numMovingCars(numMovingCars), numMovingBikes(numMovingBikes),
      numParkedCars(numParkedCars),
      numStopSigns(numStopSigns), numTrafficLights(numTrafficLights),
      simulationTicks(simulationTicks),
      minConfidenceThreshold(minConfidenceThreshold)
{
    id = seed % 10000;
    srand(seed);

    cout << "[+WORLD: " << id << "] Reticulating splines - Hello, world!" << endl;

    generateWorld();
}

// ----- Destructor -----
GridWorld::~GridWorld() {
    for (auto obj : objects) {
        delete obj;
    }
    cout << "[-WORLD: " << id << "] Goodbye, cruel world!" << endl;
}

// ----- Generate world objects -----
void GridWorld::generateWorld() {
    // Parked cars
    for (int i = 0; i < numParkedCars; ++i) {
        Position p = randomPos();
        objects.push_back(new StationaryVehicle(i, p.getX(), p.getY()));
    }

    // Stop signs
    for (int i = 0; i < numStopSigns; ++i) {
        Position p = randomPos();
        objects.push_back(new TrafficSign(i, p.getX(), p.getY()));
    }

    // Traffic lights
    for (int i = 0; i < numTrafficLights; ++i) {
        Position p = randomPos();
        objects.push_back(new TrafficLight(i, p.getX(), p.getY()));
    }

    // Moving cars
    for (int i = 0; i < numMovingCars; ++i) {
        Position p = randomPos();
        Direction d = static_cast<Direction>(rand() % 4);

        objects.push_back(new Car(i + 1, p.getX(), p.getY(), d));
    }

    // Moving bikes
    for (int i = 0; i < numMovingBikes; ++i) {
        Position p = randomPos();
        Direction d = static_cast<Direction>(rand() % 4);

        objects.push_back(new Bike(i + 1, p.getX(), p.getY(), d));
    }

    //start veh
    if (startveh.getX() >= 0 && startveh.getY() >= 0) {
        SelfDrivingCar* autoCar = new SelfDrivingCar(7,
            startveh.getX(), startveh.getY(),
            targetveh.getX(), targetveh.getY()
        );

        objects.push_back(autoCar);
    }
}

// ----- Display world -----
void GridWorld::showWorld() {
    cout << "GridWorld ID: " << id
         << ", size: " << dimX << "x" << dimY << endl;

    cout << "Objects:" << endl;
    for (auto obj : objects) {
        Position pos = obj->getPosition();
        cout << obj->getID()
             << " at (" << pos.getX() << "," << pos.getY() << ") "
             << "Glyph: " << obj->getGlyph() << endl;
    }
}


    void GridWorld::tickAll() {
    for (size_t i = 0; i < objects.size(); ++i) {
        WorldObject* obj = objects[i];
        obj->tick();

        // Έλεγχος αν είναι MovingObject
        MovingObject* mov = dynamic_cast<MovingObject*>(obj);
        if (mov) {
            Position pos = mov->getPosition();
            if (pos.getX() < 0 || pos.getX() >= dimX ||
                pos.getY() < 0 || pos.getY() >= dimY) {

                // Βγήκε εκτός → καταστρέφουμε
                cout << "[+INFO] Object " << mov->getID()
                     << " out of bounds, destroying and respawning.\n";

                delete mov;
                objects[i] = nullptr;

                // Δημιουργία νέου αντικειμένου τυχαία
                Position newPos = randomPos();
                Direction newDir = static_cast<Direction>(rand() % 4);

                // Αν είναι Car ή Bike
                if (mov->getGlyph() == 'C') {
                    objects[i] = new Car(mov->getID(), newPos.getX(), newPos.getY(), newDir);
                } else if (mov->getGlyph() == 'B') {
                    objects[i] = new Bike(mov->getID(), newPos.getX(), newPos.getY(), newDir);
                }
            }
        }
    }

    // Καθαρίζουμε τυχόν nullptr
    objects.erase(
        remove(objects.begin(), objects.end(), nullptr),
        objects.end()
    );
}

