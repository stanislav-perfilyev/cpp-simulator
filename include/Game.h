#pragma once

#include "Exceptions.h"
#include "Sector.h"

#include <array>
#include <cstddef>

/**
 * @brief Core fishing-game logic.
 *
 * The 3×3 grid holds one fish and three boots placed at random.
 * The player picks sectors (0–8) until they hit the fish (win) or
 * a boot (loss).  Both outcomes are signalled as exceptions so the
 * game loop in run() stays clean.
 */
class Game {
public:
    static constexpr std::size_t kFieldSize  = 9;
    static constexpr int         kBootCount  = 3;

    Game();

    /** @brief Run the interactive game loop (reads from std::cin). */
    void run();

    /**
     * @brief Process one player move.
     * @param sector Sector index in [0, kFieldSize).
     * @throws InvalidSectorError if @p sector is out of range.
     * @throws FishCaught         if the sector contains the fish.
     * @throws BootCaught         if the sector contains a boot.
     */
    void processInput(int sector);   // public for unit-testing

private:
    std::array<Sector, kFieldSize> field_;
    int attempts_ = 0;

    void placeFish();
    void placeBoots(int count);
};
