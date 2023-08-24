#include "square_solver.h"

SolutionSquare solve_square(const CoeffsSquare coeffs)
{
    /*
    получает структуру с коэфф и возвращает структурой результат решения с корнями
    */

    assert(isfinite(coeffs.a) && "coeff a is inf");
    assert(isfinite(coeffs.b) && "coeff b is inf");
    assert(isfinite(coeffs.c) && "coeff c is inf");

    //особые случаи
    if ( fabs(coeffs.a) < DBL_PRECISION ) //a == 0
    {
        SolutionLinear sol = solve_linear( {coeffs.b, coeffs.c} );
        return { sol.res, sol.x, 0 };
    }

    //обычные случаи, a != 0

    double discriminant = coeffs.b*coeffs.b - 4.0*coeffs.a*coeffs.c;

    if (!isfinite(discriminant)) return {ERROR_OVRFL_DISCR, 0, 0};

    if ( discriminant > DBL_PRECISION )//D > 0
    {
        double d_sqrt = sqrt(discriminant);

        double x1 = (-coeffs.b + d_sqrt) / (2.0*coeffs.a);
        double x2 = (-coeffs.b - d_sqrt) / (2.0*coeffs.a);

        if (!isfinite(x1) || !isfinite(x2)) return {ERROR_OVRFL_ROOTS, 0, 0};

        if (is_dbl_zero(x1)) x1 = fabs(x1);
        if (is_dbl_zero(x2)) x2 = fabs(x2);

        return {TWO_REAL_ROOTS, x1, x2};
    }
    else if ( is_dbl_zero(discriminant) )//D == 0
    {
        double x1 = -coeffs.b / (2.0*coeffs.a);

        if (!isfinite(x1)) return {ERROR_OVRFL_ROOTS, 0, 0};

        if (is_dbl_zero(x1)) x1 = fabs(x1);

        return {ONE_REAL_ROOT, x1, x1};
    }
    else if (discriminant < -DBL_PRECISION)//D < 0
    {
        return {ZERO_REAL_ROOTS, 0, 0};
    }

    //если все случаи учтены, программа не должна добраться до этой строчки
    assert(0 && "undefined discriminant! Unreachable line reached!");

    //по-хорошему строчка ниже никогда не должна выполняться, но иначе варнинг
    return {(SolveRes) 0, 0, 0};
}
