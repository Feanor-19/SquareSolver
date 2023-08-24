#ifndef TESTS_H
#define TESTS_H

#include "square_solver.h"
#include "utilities.h"

extern const char *TESTS_FILE_DEFAULT_NAME;

enum TestRes
{
    PASSED  = 0,
    FAILED  = 1,
    SKIPPED = 2,
};

struct TestParams
{
    CoeffsSquare test_coeffs;
    SolutionSquare test_sol;
};

//---

//читает тесты из файла, сообщает о результатах прохождения каждого теста
void run_tests(const char test_file_name[]);

#endif
