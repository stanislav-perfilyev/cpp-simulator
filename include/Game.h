#pragma once
#include "Sector.h"
#include "Exceptions.h"
#include <array>

class Game {
public:
    Game();
    void run();
private:
    std::array<Sector, 9> field;
    int attempts = 0;
    void placeFish();
    void placeBoots(int count);
    void processInput(int sector);
};
