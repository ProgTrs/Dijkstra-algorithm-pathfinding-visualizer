# Dijkstra Algorithm Pathfinding Visualizer

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-17%2B-00599C?style=for-the-badge&logo=c%2B%2B" alt="C++" />
  <img src="https://img.shields.io/badge/SDL2-2.0-000000?style=for-the-badge&logo=SDL" alt="SDL2" />
  <img src="https://img.shields.io/badge/Algorithm-Dijkstra-4CAF50?style=for-the-badge" alt="Dijkstra" />
</p>

A visually interactive terrain-based shortest-path explorer built in C++ using Dijkstra's algorithm. The project renders a 2D height map, lets you click a start and end tile, then highlights the optimal route while visually encoding elevation data across the map.

This is not just a textbook pathfinding demo: it turns the grid into a simulated landscape with depth, cursor profiling, and real-time visualization of explored nodes and final route.

## Why this project is interesting

This project combines:

- graph search and shortest-path logic
- terrain visualization using color and height encoding
- interactive map editing through mouse clicks
- custom SDL-based graphics utilities
- search-space rendering to echo how an algorithm explores the map

At a high level, the program loads a matrix from a text file, treats each cell as a node, runs Dijkstra's algorithm over valid land tiles, and draws the results directly to the window.

## Features

- Click-to-select start point and destination
- Real-time shortest-path calculation using Dijkstra
- 8-directional movement with weighted costs (orthogonal = 10, diagonal = 14)
- Terrain rendering using elevation values
- Search-space highlighting after completion
- Final path highlighting in bright color
- Bottom/right profile panels tracking cursor elevation
- Status box showing whether the path is reachable and how many steps it took
- Generates terrain maps via a procedural generator script

## Algorithm overview

The core search logic lives in `main.cpp` inside the `VisualMap` class.

The implementation uses a priority queue to perform Dijkstra's shortest path search with these characteristics:

- Start node: `point_A`
- Goal node: `point_B`
- Walkable tiles: values `m[j][i] > 0`
- Non-walkable/negative tiles: treated as water or blocked terrain
- Neighbor directions: 8-connected graph, allowing diagonal movement
- Weight model:
  - horizontal/vertical step: 10
  - diagonal step: 14

This is a classic grid shortest-path problem where each move has a cost proportional to distance. The route is then reconstructed from the destination back to the origin by following parent links implied by the best-known distances.

## Project structure

```text
Dijkstra-algorithm-pathfinding-visualizer/
├── CMakeLists.txt          # Build configuration
├── README.md               # Project documentation
├── README                  # Legacy root README stub
├── graphics.cpp            # Low-level graphics backend implementation
├── graphics.hpp            # graphics API / window abstraction
├── char2.h                 # Additional utility/header support file
├── keytbl.h                # Keyboard table or input support
├── main.cpp                # Main interactive visualizer
├── main2.cpp               # Terrain generation script
├── notmain.cpp             # Experimental/alternate demo code
├── include/                # Project include headers
├── lib/                    # Local library files and dependencies
├── .idea/                  # JetBrains IDE metadata
└── terkep.txt              # Terrain file used by main program (generated or provided)
```

## What the code does

### `main.cpp`

This is the primary application.

The `VisualMap` class handles:

- reading the map from a text file
- creating the SDL canvas
- drawing terrain tiles and overlays
- responding to mouse clicks
- running the Dijkstra search
- displaying the result and status information

Key flow:

1. The program loads `terkep.txt`
2. A grid is created from the file contents
3. The user clicks two cells on the map
4. `search_shortest_distance2()` computes the path
5. `display()` repaints the map and overlays the route
6. A blue/green status panel reports search outcome

### `main2.cpp`

This file generates terrain data. It creates a synthetic height map with random noise, smooths it, rescales it, and writes the result to `terkep.txt`.

This is useful if you want a fresh terrain layout instead of a static dataset.

### `graphics.hpp` / `graphics.cpp`

This project includes a custom graphics library built around SDL. It wraps low-level rendering and window logic so the app can draw boxes, lines, colors, and text without needing a large third-party UI framework.

The library provides a highly lightweight imperative graphics API, including:

- `canvas`
- `groutput`
- `gout` global instance
- manipulators like `color`, `move_to`, `box`, `line_to`, `text`

### `char2.h` and `keytbl.h`

These files appear to provide extra character/input support and key mapping utilities for the graphics abstraction. They are part of the supporting ecosystem around the custom rendering environment.

## Controls

- Left click on a valid land tile to place the start node
- Left click again on another valid tile to place the destination
- The program automatically recalculates the path after the second click
- The mouse cursor triggers profile panes for the current row and column
- The status panel at the side indicates whether a path exists

## How the terrain works

The grid is read as integers, where each cell is interpreted as an elevation or terrain value.

- positive values are traversable terrain
- negative or zero-like values are treated as water/blocked areas
- the visualizer shades terrain based on these values
- the final path is highlighted using a golden/orange color

## Build and run

### Prerequisites

This project expects a C++ environment with SDL2 support.

On Linux/macOS, install the required development packages, for example:

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install build-essential cmake libsdl2-dev libsdl2-ttf-dev
```

On Windows, this project is typically intended for a setup with SDL2 headers and libraries available in the project environment or a compatible CLion/MSYS/MinGW toolchain.

### Configure with CMake

```bash
cmake -S . -B build
```

### Build

```bash
cmake --build build
```

### Run

The project currently builds an executable named `CLionGraphicsKit` from the CMake file, which is the visualizer entry point.

```bash
./build/CLionGraphicsKit
```

If the program cannot find `terkep.txt`, generate a new map first using the generation utility or place a valid terrain file in the project root.

## Generating a map

The repository includes `main2.cpp`, which generates a synthetic `terkep.txt` file.

The typical flow is:

1. compile the generator or adapt it into the build
2. run it to create `terkep.txt`
3. run the main visualizer

This is especially useful for experimentation with different terrain layouts and obstacle distributions.

## Important technical notes

This repository is an older C++ graphics project with a somewhat custom dependency setup. A few things to keep in mind:

- `CMakeLists.txt` uses `CMAKE_CXX_STANDARD 26`, which is unusually modern for a small SDL app and may require a recent compiler
- the project links `SDL2`, `SDL2.dll`, `SDL2main`, `SDL2_ttf`, and `SDL2_ttf.dll`
- multiple source files exist (`main.cpp`, `main2.cpp`, `notmain.cpp`), so the active entry point depends on the target configuration
- some code appears to be experimental or historical, and not all files are part of the final app

## Example usage flow

```text
Generate terrain -> Load map -> Click start -> Click destination -> View path -> Analyze explored cells
```

## Why Dijkstra was a good choice here

Dijkstra's algorithm is ideal for a weighted, grid-based terrain search because it guarantees the shortest path when all edge costs are non-negative. In this application, diagonal movement and directional penalties make the problem resemble a weighted shortest-path problem rather than a pure unweighted BFS scenario.

## Potential improvements

If you want to extend this project further, some natural upgrades would be:

- add A* optimization for faster route generation
- support dragging to paint walls and obstacles
- add path animation
- add a GUI for terrain controls and map resizing
- add real-time search legend and metrics
- support saving/loading custom maps
- add multi-source or dynamic path recalculation

## License

No explicit license was specified in the repository metadata, so the code is currently distributed without a declared license. If you're planning to use or publish this project commercially, it is wise to check the repository owner before redistribution.

## Summary

This project is a compact, artistic pathfinding visualizer that demonstrates both algorithmic thinking and custom graphics programming. It is especially interesting because it pairs a standard algorithm with a compelling visual representation of a weighted map, turning an abstract shortest-path problem into an interactive terrain exploration tool.

If you want to experiment, build it, generate new maps, and click around the scene to see the route evolve in real time.

---

<p align="center">
  <i>Built for algorithm visualization, terrain exploration, and interactive pathfinding.</i>
</p>
