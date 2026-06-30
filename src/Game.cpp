#include "Game.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

Game::Game()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    placeFish();
    placeBoots(kBootCount);
}

void Game::placeFish()
{
    const int pos = std::rand() % static_cast<int>(kFieldSize);
    field_[pos].fish = std::make_unique<Fish>();
}

void Game::placeBoots(int count)
{
    int placed = 0;
    while (placed < count) {
        const int pos = std::rand() % static_cast<int>(kFieldSize);
        if (field_[pos].fish || field_[pos].boot) continue;
        field_[pos].boot = std::make_unique<Boot>();
        ++placed;
    }
}

void Game::processInput(int sector)
{
    if (sector < 0 || sector >= static_cast<int>(kFieldSize))
        throw InvalidSectorError(sector);

    ++attempts_;
    Sector& s = field_[sector];
    if (s.fish) throw FishCaught(attempts_);
    if (s.boot) throw BootCaught(attempts_);
    std::cout << "Empty! Try again.\n";
}

void Game::run()
{
    try {
        while (true) {
            std::cout << "Enter sector (0-" << kFieldSize - 1 << "): ";
            int s = 0;
            if (!(std::cin >> s)) {
                std::cout << "Input error.\n";
                return;
            }
            try {
                processInput(s);
            } catch (const InvalidSectorError& e) {
                std::cout << e.what() << ". Please enter 0-"
                          << kFieldSize - 1 << ".\n";
            }
        }
    } catch (const FishCaught& fc) {
        std::cout << "Success! Fish caught in " << fc.attempts << " attempt(s).\n";
    } catch (const BootCaught& bc) {
        std::cout << "Failure. Boot caught in " << bc.attempts << " attempt(s).\n";
    }
}
