Project Group Information
=========================

Student Name: Ahmad Soboh
Student Number: 300294253

Student Name: Pamir Habib 
Student Number: 300284009

Course Code: CSI2372A

Project Description
-------------------
Memoarrr! console implementation featuring base rules, expert display, and expert-rule variants as described in the assignment specification.
# Project_CSI2372A_F2025

## Requirements

- CMake 3.16+
- A C++14 toolchain (Visual Studio 2019+ on Windows, or any compiler with C++14 support)

## Configure & Build

Ensure you are in project root folder
```cmd
cmake -S . -B build
cmake --build build
```

## Run

```cmd
build\Debug\memoarrr.exe
```

During startup the executable prompts for:

1. Display mode (base 5x5 grid or expert row display)
2. Rules mode (base rules or expert animal abilities)
3. Number of players (2–4), player names, and seat selection (top/right/bottom/left)

Each round automatically resets the board, lets every player secretly peek at the three cards in front of their seat, then runs the full Memoarrr! turn sequence including ruby awards. Expert-mode abilities (octopus swap, penguin flip-down, walrus block, crab extra flip, turtle skip) can be combined with either display option.