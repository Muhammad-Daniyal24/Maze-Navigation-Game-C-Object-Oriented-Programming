# Maze Runner - C++ OOP Console Game

A text-based maze runner game built in C++ using Object-Oriented Programming concepts. The player navigates through a 2D maze, avoids obstacles, collects rewards, and tries to reach the finish point with the best possible performance.

This project was developed as a semester project for the Object-Oriented Programming course at GIK Institute.

## Repository Contents

This repository contains:

- `main.cpp` - main source file containing the game logic and execution flow
- `include/` - header files/classes used in the project
- `Project Documentation` - detailed project proposal/documentation explaining the design, features, OOP concepts, class structure, and flowchart
- `README.md` - project overview and run instructions

## Project Description

Maze Runner is a console-based game where the maze is displayed using ASCII characters. The player moves using keyboard inputs and tries to reach the finish cell while interacting with different maze elements such as walls, traps, treasures, and enemies.

The game also tracks the player’s number of moves, score, and time taken. At the end of the game, player information can be saved using file handling.

## Features

- 2D maze navigation in the console
- Keyboard-based player movement
- Walls that block movement
- Traps that penalize the player
- Treasures that increase the score
- Enemies that add difficulty
- Finish cell to complete the game
- Move counter
- Timer for completion time
- Player name input
- Score/result saving using file handling
- ASCII-based text interface

## Controls

| Key | Action |
|-----|--------|
| `W` | Move Up |
| `A` | Move Left |
| `S` | Move Down |
| `D` | Move Right |
| `Q` | Quit Game |

## Maze Symbols

| Symbol | Meaning |
|--------|---------|
| `#` | Wall |
| `P` | Player |
| `T` | Trap |
| `$` | Treasure |
| `E` | Enemy |
| `F` | Finish / Exit |

## OOP Concepts Used

The project applies the following Object-Oriented Programming concepts:

- **Classes and Objects** - used to model the player, maze, game, and maze elements
- **Encapsulation** - player data, maze structure, and game logic are organized inside classes
- **Inheritance** - different tile types are created from a common base tile structure
- **Polymorphism** - different maze elements behave differently when the player interacts with them
- **Abstraction** - complex game operations are handled through simple class methods
- **File Handling** - player results are saved for record keeping

## How to Run

Make sure you have `g++` installed.

### Compile

```bash
g++ main.cpp -o maze_runner
