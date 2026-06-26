#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Game::Game() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    placeFish();
    placeBoots(3);
}

void Game::placeFish() {
    int pos = std::rand() % field.size();
    field[pos].fish = std::make_unique<Fish>();
}

void Game::placeBoots(int count) {
    int placed = 0;
    while (placed < count) {
        int pos = std::rand() % field.size();
        if (field[pos].fish || field[pos].boot) continue;
        field[pos].boot = std::make_unique<Boot>();
        ++placed;
    }
}

void Game::processInput(int sector) {
    ++attempts;
    Sector &s = field[sector];
    if (s.fish) throw FishCaught(attempts);
    if (s.boot) throw BootCaught(attempts);
    std::cout << "Empty! Try again.\n";
}

void Game::run() {
    try {
        while (true) {
            std::cout << "Enter sector (0-8): ";
            int s;
            if (!(std::cin >> s)) {
                std::cout << "Enter Error.\n";
                return;
            }
            if (s < 0 || s >= static_cast<int>(field.size())) {
                std::cout << "Number incorrect.\n";
                continue;
            }
            processInput(s);
        }
    } catch (const FishCaught &fc) {
        std::cout << "Success! Fish caught for " << fc.attempts << " attempts.\n";
    } catch (const BootCaught &bc) {
        std::cout << "Failure. Boot caught for " << bc.attempts << " attempts.\n";
    }
}
