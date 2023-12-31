#ifndef INPUT_SQUARE_SOLVER
#define INPUT_SQUARE_SOLVER

#include "utilities.h"

/*!
    @brief Ask user to enter coefficients for the square equation to solve.

    Ask user to enter coefficients, check input,
    return coefficients as double packed in a struct CoeffsSquare.*.

    @return Coefficients of a quadratic equation packed in a struct CoeffsSquare entered by user.
*/
CoeffsSquare get_input(void);

#endif

