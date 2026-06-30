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

Build (command line)
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
