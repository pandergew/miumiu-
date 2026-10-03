#include <iostream>
#include "GridWorld.h"
#include "Objects.h"

using namespace std;

int main() {
    cout << "=== Starting Simulation ===" << endl;

    // Δημιουργία κόσμου με default constructor
    GridWorld world;

    cout << "\n=== Initial World State ===" << endl;
    world.showWorld();

    cout << "\n=== Simulation Ticks ===" << endl;

    // Απλό simulation loop
    for (int tick = 1; tick <= 5; ++tick) {
        cout << "\n--- Tick " << tick << " ---" << endl;

        // Κάνε tick σε όλα τα objects
        // (αν το έχεις public ή μέσω function)
        // Αν δεν έχεις ακόμα world.tick(), το αφήνουμε έτσι

        world.tickAll();
        world.showWorld();
    }

    cout << "\n=== Ending Simulation ===" << endl;
    return 0;
}
