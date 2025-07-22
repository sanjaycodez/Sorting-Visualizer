# Sorting Visualizer

A modern, interactive C++ application for visualizing popular sorting algorithms in real-time using [Raylib](https://www.raylib.com/). This project is designed for learning, demonstration, and experimentation with sorting algorithms and their behaviors.

## Features
- **Visualize multiple sorting algorithms**: Bubble Sort, Insertion Sort, Quick Sort, Merge Sort
- **Interactive UI**: Select algorithms, adjust array size and sorting speed, randomize the array, and start/stop sorting
- **Resizable window**: Responsive visualization for different screen sizes
- **Smooth animations**: Powered by Raylib and C++ threading
- **Easy to extend**: Add your own algorithms or tweak the UI

## Supported Sorting Algorithms
- Bubble Sort
- Insertion Sort
- Quick Sort
- Merge Sort

## Controls & Usage
- **SORTING**: Click to open the sorting algorithm selection menu
- **Algorithm Buttons**: Choose the algorithm you want to visualize
- **Start Sorting!**: Begin the visualization for the selected algorithm
- **Stop Sorting!**: Interrupt the current sorting process
- **Randomize Array!**: Shuffle the array for a new sorting challenge
- **Speed +/-**: Increase or decrease the sorting animation speed
- **Size +/-**: Increase or decrease the number of elements (pillars) in the array

> **Tip:** After a sort completes, you can open the sorting menu again to select and visualize a different algorithm without restarting the program.

## Build & Run Instructions (macOS)

### Prerequisites
- **C++ Compiler** (e.g., g++)
- **Raylib** (Install via Homebrew)

```sh
brew install raylib
```

### Compile & Run

By default, the Makefile is set up for `SortingVisualizer.cpp`.

```sh
make
./SortingVisualizer
```

If you want to compile manually:

```sh
g++ SortingVisualizer.cpp -o SortingVisualizer -I/opt/homebrew/include -L/opt/homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
```

## Project Structure

- `SortingVisualizer.cpp` — Main source file
- `Makefile` — For easy compilation

## Extending & Contributing
- Fork the repo and submit pull requests for new features or improvements
- Add new sorting algorithms by following the structure in `SortingVisualizer.cpp`
- Issues and suggestions are welcome!

## License
This project is open source and available under the MIT License. 