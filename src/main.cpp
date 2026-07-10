#include "Exceptions.h"
#include "Game.h"

#include <iostream>

int main()
{
    Game game;
    try {
        while (true) {
            std::cout << "Enter sector (0-" << Game::kFieldSize - 1 << "): ";
            int s = 0;
            if (!(std::cin >> s)) {
                std::cout << "Input error.\n";
                return 1;
            }
            try {
                game.processInput(s);
            } catch (const InvalidSectorError& e) {
                std::cout << e.what() << ". Please enter 0-"
                          << Game::kFieldSize - 1 << ".\n";
            }
        }
    } catch (const FishCaught& fc) {
        std::cout << "Success! Fish caught in " << fc.attempts << " attempt(s).\n";
    } catch (const BootCaught& bc) {
        std::cout << "Failure. Boot caught in " << bc.attempts << " attempt(s).\n";
    }
    return 0;
}
