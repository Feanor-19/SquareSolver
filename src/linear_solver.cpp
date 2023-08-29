#include "linear_solver.h"

SolutionLinear solve_linear(const CoeffsLinear coeffs)
{
    if ( fabs(coeffs.a) < DBL_PRECISION ) //a == 0
    {
        if ( fabs(coeffs.b) < DBL_PRECISION )//b == 0
        {
            return {INFINITE_SOLUTIONS, 0};
        }
        else //b != 0
        {
            return {NO_SOLUTION, 0};
        }
    }
    else //a != 0
    {
        double x1 = -coeffs.b/coeffs.a;

        if (!isfinite(x1)) return {ERROR_OVRFL_ROOTS, 0};

        if (is_dbl_zero(x1)) x1 = fabs(x1); //лекарство от -0

        return {LINEAL_ROOT, x1};
    }
}
