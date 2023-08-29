#ifndef SQUARE_SOLVER_H
#define SQUARE_SOLVER_H

#include "utilities.h"
#include "linear_solver.h"

/*!
    @brief Contains solution of a quadratic equation.
*/
struct SolutionSquare
{
    SolveRes res; ///< Solution result code as a SolveRes enum member.
    double x1;    ///< First root (if exists).
    double x2;    ///< Second root (if exists).
};

//---

/*!
    @brief Solve quadratic equation ax^2 + bx + c = 0.

    @param [in] coeffs Coefficients of a quadratic equation packed in a struct CoeffsSquare
    @return Solution of the quadratic equation packed in a struct SolutionSquare.
*/
SolutionSquare solve_square(const CoeffsSquare coeffs);

#endif
