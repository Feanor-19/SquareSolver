#ifndef TESTS_H
#define TESTS_H

#include "square_solver.h"
#include "utilities.h"

extern const char *TESTS_FILE_DEFAULT_NAME;

/*!
    @brief Contains all possible test results.
*/
enum TestRes
{
    PASSED  = 0, ///< Fully passed.
    FAILED  = 1, ///< Failed.
    SKIPPED = 2, ///< Skipped for some reason. Common reason is a wrong test itself.
};

/*!
    @brief Contains coefficients and solution from a unit-test quadratic equation.
*/
struct TestParams
{
    CoeffsSquare test_coeffs; ///< Coefficients from a unit-test packed as CoeffsSquare struct.
    SolutionSquare test_sol;  ///< Solution from a unit-test packed as SolutionSquare struct.
};

//---

/*!
    @brief Main function to run tests.

    @param [in] test_file_name Name of the file containing tests.
*/
void run_tests(const char test_file_name[]);

#endif
