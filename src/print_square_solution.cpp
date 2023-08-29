#include "print_square_solution.h"

void print_square_solution(SolutionSquare solution)
{
    switch (solution.res)
    {
        case TWO_REAL_ROOTS:
            printf("Here are two roots: %lg %lg\n", solution.x1, solution.x2);
            break;
        case ONE_REAL_ROOT:
            printf("Here are two identical roots: %lg %lg\n", solution.x1, solution.x2);
            break;
        case ZERO_REAL_ROOTS:
            printf("There are no real roots... And I don't have imagination "
            "to compute complex ones.\n");
            break;
        case ERROR_OVRFL_ROOTS:
            printf("Overflow during computing roots detected! Unable to compute roots.\n");
            break;
        case ERROR_OVRFL_DISCR:
            printf("Overflow during computing discriminant detected! Unable to compute roots.\n");
            break;
        case NO_SOLUTION:
            printf("No solution. I guess both a and b are zeros, and c is not zero.\n");
            break;
        case LINEAL_ROOT:
            printf("It is not ax^2+bx+c=0, it is bx+c=0. x = -c/b. x = %lg.\n", solution.x1);
            break;
        case INFINITE_SOLUTIONS:
            printf("Well, any real number is a solution. But a = b = c = 0 is too trivial, you know.\n");
            break;
        default:
            assert(0 && "Somehow function \"solve\" returned unsupported value...\n");
    }
}
