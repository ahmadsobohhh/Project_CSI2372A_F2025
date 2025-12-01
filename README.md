# Project_CSI2372A_F2025

Memoarrr! console implementation for CSI2372A Fall 2025. The program supports the full base game plus both expert variants (expert display and expert rules) exactly as described in the assignment brief.

## Requirements

- CMake 3.16+
- A C++14 toolchain (Visual Studio 2019+ on Windows, or any compiler with C++14 support)

## Configure & Build

```cmd
cd "C:\Users\Ahmad Soboh\Desktop\Projects\Project_CSI2372A_F2025"
cmake -S . -B build
cmake --build build
```

## Run

```cmd
cd "C:\Users\Ahmad Soboh\Desktop\Projects\Project_CSI2372A_F2025"
build\Debug\memoarrr.exe
```

During startup the executable prompts for:

1. Display mode (base 5x5 grid or expert row display)
2. Rules mode (base rules or expert animal abilities)
3. Number of players (2–4), player names, and seat selection (top/right/bottom/left)

Each round automatically resets the board, lets every player secretly peek at the three cards in front of their seat, then runs the full Memoarrr! turn sequence including ruby awards. Expert-mode abilities (octopus swap, penguin flip-down, walrus block, crab extra flip, turtle skip) can be combined with either display option.

## Submission Notes

- Fill in `README.txt` with your group information before creating the submission ZIP, as required by the assignment instructions.
- Source files are organized under `include/` and `src/`; `CMakeLists.txt` builds the `memoarrr` console executable.
