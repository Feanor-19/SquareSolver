#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <ctype.h>

/*!
    @brief Contains coefficients of a quadratic equation ax^2 + bx + c = 0.
*/
struct CoeffsSquare
{
    double a; ///< Coefficient a.
    double b; ///< Coefficient b.
    double c; ///< Coefficient c.
};

/*!
   @brief Contains all possible solution result codes.

   @note Must contain only consecutive values (e.g. 0, 1, 2, but not 0, 2, 3)
*/
enum SolveRes
{
    ZERO_REAL_ROOTS     = 0, ///< Zero real roots.
    ONE_REAL_ROOT       = 1, ///< One real root.
    TWO_REAL_ROOTS      = 2, ///< Two real roots.
    NO_SOLUTION         = 3, ///< No solution (even imaginary).
    LINEAL_ROOT         = 4, ///< Lineal root. (a = 0)
    INFINITE_SOLUTIONS  = 5, ///< Infinite number of solutions (a = b = c = 0)
    ERROR_OVRFL_DISCR   = 6, ///< Error in case of overflow during computing discriminant.
    ERROR_OVRFL_ROOTS   = 7, ///< Error in case of overflow during computing roots.
};
///@brief Must be equal to the least value of all SolveRes members
const int SOLVE_RES_MIN_CODE = 0; //ДОЛЖНО ОБНОВЛЯТЬСЯ В СООТВЕТСТВИИ С enum SolveRes
///@brief Must be equal to the maximum value of all SolveRes members
const int SOLVE_RES_MAX_CODE = 6; //ДОЛЖНО ОБНОВЛЯТЬСЯ В СООТВЕТСТВИИ С enum SolveRes

//-----

/*!
    @brief Contains possible codes for main() to return, is held in GLOBAL_CURRENT_RETURN_CODE.
*/
enum MainReturnCode
{
    NO_ERRORS = 0, ///< Default value.
    EOF_FOUND = 1, ///< Is used when EOF is found instead of normal input.
};

//ниже глобальная переменная, в которой хранится нынешнее возвращаемое всей программой значение;
//оно может поменяться только один раз, и как только оно становится не NO_ERRORS, программа как можно скорее
//добирается до main (если не уже там) и возвращает данный код ошибки (все это сделано, чтобы избавиться от abort и exit)
extern MainReturnCode GLOBAL_CURRENT_RETURN_CODE;

#define DEF_CHECK_CURRENT_RETURN_CODE { if (GLOBAL_CURRENT_RETURN_CODE != NO_ERRORS) return GLOBAL_CURRENT_RETURN_CODE; }

//---

/// @brief Used in print_stars() for decoration purposes in printing.
const int STARS_STRIP_WIDTH = 40;
/// @brief Double precision.
const double DBL_PRECISION = DBL_EPSILON * 10;
/// @brief Maximum length for the name of a custom test file.
const int MAX_CUSTOM_TEST_FILE_NAME = 100;

//---

/*!
    @brief Prints *number* stars in stdout for decoration purposes.

    @param [in] number Number of stars to print.
*/
void print_stars(int number);

/*!
    @brief Checks if two double values are equal.

    @param [in] x, y Two double values to check.
    @return 1 if true, 0 otherwise.
*/
int are_dbls_equal(double x, double y);

/*!
    @brief Checks if double value is close enough to zero.

    @param [in] x Double value to check.

    @return 1 if true, 0 otherwise.
*/
int is_dbl_zero(const double x);

/*!
    @brief Clears stdin until '\n' is met.
*/
void clear_buf(void);

/*!
    @brief Copies symbols from file *from* to file *to* until '\n' is met.

    @param [in] from File to copy from.
    @param [in] to File to copy to.
*/
void echo_line(FILE* from, FILE* to);

/*!
    @brief Skips all space symbols in stdin.
*/
void skip_spaces(void);

#endif
