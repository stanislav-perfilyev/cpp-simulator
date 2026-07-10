# Fish Catch (Task2)

Description
- Simple console fish-catching simulation in C++ (C++20). The pond has 9 sectors.
- One fish and three boots are placed randomly in different sectors at game start.
- Player inputs a sector number (1..9) to cast the line.
- Catching the fish throws a user exception `FishCaught` with the attempt count and ends the game successfully.
- Catching a boot throws `BootCaught` and ends the game with failure.
- Empty sector: the player can try again.

Requirements
- C++ compiler with C++20 support.
- CMake.
- Recommended: CLion on Windows (project prepared for Windows), but command-line build works too.

Project structure
- `CMakeLists.txt` — build configuration.
- `include/` — header files.
- `src/` — source files (`src/main.cpp`, `src/Game.cpp`, ...).
- `README.md` — this file.
- Executable target name: `FishCatch` (set in `CMakeLists.txt`).

## Architecture

The project is structured into three layers:

- **Domain** (`include/Fish.h`, `include/Boot.h`, `include/Sector.h`): plain value types representing pond objects.
- **Game logic** (`src/Game.cpp`, `include/Game.h`): `Game` manages a 9-sector field, places objects randomly, and processes player input by throwing typed exceptions (`FishCaught`, `BootCaught`, `InvalidSectorError`).
- **Exception hierarchy** (`include/Exceptions.h`): all game events derive from `SimulatorError` → `std::exception`, enabling structured catch chains.

Control flow uses exceptions intentionally to signal game-ending events, keeping `processInput()` free of return codes.

## Usage

Run the compiled executable and enter sector numbers (0–8) to fish:

```
./fishing_simulator
Enter sector (0-8): 3
Empty! Try again.
Enter sector (0-8): 7
Success! Fish caught in 2 attempt(s).
```

## Build
1. Create build directory:
    - `mkdir build`
    - `cd build`
2. Configure:
    - `cmake ..`
3. Build:
    - `cmake --build . --config Release`

Run
- Windows (Release): `.\Release\FishCatch.exe`
- Other generators: `./FishCatch`

Usage
- Program reads integer sector numbers from standard input (1..9).
- Invalid or out-of-range input is handled and prompts for retry.
- On catching a fish the program reports attempts used.
- On catching a boot the program reports failure and exits.

Implementation notes
- Random placement uses a random generator to place one fish and three boots into distinct sectors.
- Exceptions are used to signal catching events (`FishCaught`, `BootCaught`).
- Source/headers are located in `src/` and `include/` respectively.
