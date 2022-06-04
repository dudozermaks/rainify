# No rain outside the window? Rainify is your solution!

## Demonstration
![Demonstration](https://github.com/dudozermaks/dudozermaks.github.io/blob/master/assets/gifs/rainify_1.0.0.gif)

> Gif is compressed and FPS downgraded

## How-to build and run:

### To build (no Windows because of ncurses):

Deps:

* [CMake](https://cmake.org/)
* [ncurses](https://ru.wikipedia.org/wiki/Ncurses) (most unix-based systems already have)

1. Clone repo
2. Go to repo dir
3. `mkdir build && cd build`
4. `cmake ..`
5. `cd .. && cmake --build build/`

### To run:

1. `./build/main`
