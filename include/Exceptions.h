#pragma once

#include <stdexcept>
#include <string>

/**
 * @brief Base exception for all simulator errors.
 *
 * Hierarchy:
 *   SimulatorError
 *   ├── FishCaught        — player found the fish (success outcome)
 *   ├── BootCaught        — player found a boot  (failure outcome)
 *   └── InvalidSectorError — sector index out of range [0, 8]
 *
 * Note: FishCaught / BootCaught are used here as structured flow-control
 * signals to exit the game loop cleanly.  They carry the attempt count so
 * the caller does not need to track it separately.
 */
class SimulatorError : public std::exception {
public:
    explicit SimulatorError(const char* what_arg) : what_(what_arg) {}
    explicit SimulatorError(const std::string& what_arg) : what_(what_arg) {}
    [[nodiscard]] const char* what() const noexcept override { return what_.c_str(); }
private:
    std::string what_;
};

/** @brief Thrown when the player finds the fish (game won). */
class FishCaught : public SimulatorError {
public:
    explicit FishCaught(int attempts)
        : SimulatorError("Fish caught!"), attempts(attempts) {}
    const int attempts;
};

/** @brief Thrown when the player finds a boot (game lost). */
class BootCaught : public SimulatorError {
public:
    explicit BootCaught(int attempts)
        : SimulatorError("Boot caught!"), attempts(attempts) {}
    const int attempts;
};

/** @brief Thrown when an invalid sector index is passed to processInput(). */
class InvalidSectorError : public SimulatorError {
public:
    explicit InvalidSectorError(int sector)
        : SimulatorError("Invalid sector: " + std::to_string(sector))
        , sector(sector) {}
    const int sector;
};
