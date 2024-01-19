# Snake AI 

## Introduction
An attempt to create to create a working AI using a version of the NEAT algorithm that uses complex neurons to beat the game Snake and other popular games in the future. This project is currently being developed on MacOS and Ubuntu; compatibility with other environment is not guaranteed nor actively tested. 

## Build
### Dependencies
This project requires a working BLAS implementation. The project is configured to use [OpenBLAS](https://github.com/OpenMathLib/OpenBLAS) by default, and currently, there isn't an easy way to configure the project to use any alternative BLAS implementation. 

### Normal Compile
This project uses CMake to manage dependencies and build so you must have CMake installed. In addition, a working C compiler compatible with C99 must also be installed. To build the project, run

1. `mkdir build`
2. `cd build`
3. `cmake ..`
4. `make`

Then to test the project, run `ctest` or `make test` in the build directory.

👍😁
