#ifndef SQUARE_SOLVER_H
#define SQUARE_SOLVER_H

#include "utilities.h"
#include "linear_solver.h"

struct SolutionSquare
{
    SolveRes res;
    double x1;
    double x2;
};

//---

//получает коэфф квадратного уравнения и возвращает результат решения с корнями
SolutionSquare solve_square(const CoeffsSquare coeffs);

#endif
