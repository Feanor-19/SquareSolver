# Quadratic equations solver

## Build
Repo includes Makefile. Build with `make`.

## Usage
Type in terminal: `.\square_solver.exe`

### Available flags:

1. `-t` - enables tests. Tests are read from default file "SquareSolverTests.txt".
2. `-cstm-tests` - works only with `-t` enabled. Enter name of your file with tests.

### Available return values:

`0` - `NO ERRORS`

`1` - `EOF FOUND`

This return codes are specified in enum `MainReturnCode`.

## Credits
This is MIPT summer school project. Thanks to my mentor PLT!
