# Checkers3D
A little side project to learn OpenGL, graphics programming and the necessary mathematics

## Setup and Requirements
Currently, only Ubuntu (specifically version 22.04.5) is tested and actively supported.<br/>
After acquiring the repository files (using `git clone` or downloading the zip) run `make setup-project`.
This will:
- download and install globally all the necessary packages using `apt`,
- download and setup [`vcpkg`](https://github.com/microsoft/vcpkg) locally,
- create build files using [`cmake`](https://cmake.org/)

After that, execute `make compile && make run` or just `make` for short to compile the project and run the game.

