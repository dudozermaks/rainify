# No rain outside the window? Rainify is your solution!

## Demonstration
![Demonstration](https://github.com/dudozermaks/dudozermaks.github.io/blob/master/assets/gifs/rainify_2.0.0.gif)

> Gif is compressed and FPS downgraded

## How-to build and run (no Windows because of ncurses):

### To build:

Deps:

* [CMake](https://cmake.org/)
* [ncurses](https://en.wikipedia.org/wiki/Ncurses) (most unix-based systems already have)

1. Clone repo
2. Go to repo dir
3. `mkdir build && cd build`
4. `cmake ..`
5. `cd .. && cmake --build build/`

### To run:

1. `./build/main`

---

Args parser with [argpars](https://github.com/p-ranav/argparse) <br/>
TUI (text user interface) with [ncurses](https://en.wikipedia.org/wiki/Ncurses)
