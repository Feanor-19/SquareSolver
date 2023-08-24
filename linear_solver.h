#ifndef LINEAR_SOLVER_H
#define LINEAR_SOLVER_H

#include "utilities.h"

struct CoeffsLinear
{
    double a;
    double b;
};

struct SolutionLinear
{
    SolveRes res;
    double x;
};

//---

//получает коэфф линейного уравнения и возвращает результат решения с корнями
SolutionLinear solve_linear(const CoeffsLinear coeffs);

#endif
