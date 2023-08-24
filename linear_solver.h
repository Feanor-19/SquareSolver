#ifndef LINEAR_SOLVER_H
#define LINEAR_SOLVER_H

#include "utilities.h"

/*!
    @brief Contains coefficients of a linear equation ax + b = 0.
*/
struct CoeffsLinear
{
    double a; ///< Coefficient a.
    double b; ///< Coefficient b.
};

/*!
    @brief Contains solution of a linear equation ax + b = 0.
*/
struct SolutionLinear
{
    SolveRes res;   ///< Solution result code from SolveRes enum.
    double x;       ///< The root of the equation (if exists).
};

//---

/*!
    @brief Solves linear equation ax + b = 0.

    @param [in] coeffs Coefficients of the linear equation packed in a struct CoeffsLinear.
    @return Solution of the linear equation packed in a struct SolutionLinear.
*/
SolutionLinear solve_linear(const CoeffsLinear coeffs);

#endif
