#
## [The Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)

## Rules
1. Any live cell with fewer than two live neighbours dies, as if by underpopulation.
2. Any live cell with two or three live neighbours lives on to the next generation.
3. Any live cell with more than three live neighbours dies, as if by overpopulation.
4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.

## Dependencies
- [raylib](https://github.com/raysan5/raylib)
- CMake 3.10 or newer

## How to build and run
1. `git clone https://github.com/notfoun9/game_of_life.git`
2. `cd game_of_life`
3. `mkdir build && cd build`
4. `cmake .. && make`
5. Run with `./main`

## Implemented features
- Pause/Unpause by pressing Space
- When paused press N to live one more age
- Add life with Left Mouse button
- Erase life with Right Mouse button
- Erase all life by pressing C
- Change unpaused life speed by pressing Numbers 1 - 9
- Toggle grid rendering by pressing G (Not rendering, Above life or Under life)

## TODO
- Make game window resizable
- Add ability to set cell size
- Add ability to set colors (also for cells that will be alive/dead the next age)
- Add ability to save current state snapshot (one or multiple)
