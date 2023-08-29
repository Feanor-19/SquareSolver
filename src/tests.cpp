#include "tests.h"

/// @brief Holds default name for file containing tests.
const char *TESTS_FILE_DEFAULT_NAME = "SquareSolverTests.txt";

/*!
    @brief Runs one test.

    @param [in] params Params of the test, including coefficients and reference solution.
    @param [in] test_num Number of test, used in printing.

    @return Result of test as a member of TestRes enum.
*/
static TestRes run_one_test(const TestParams params, const int test_num);

void run_tests(const char test_file_name[])
{
    printf( "If you didn't specify tests file name "
            "using -cstm-test, please create file with name \"%s\" "
            "in the same folder this program runs in to "
            "do automatic tests. Anyway, every line of the test file should include "
            "three real numbers separated by spaces (square equation coefficients), "
            "solution code (see below) and from zero to two roots (depends on "
            "solution code).\nSolution codes:\n"
            "ZERO_REAL_ROOTS    = 0,\n"
            "ONE_REAL_ROOT      = 1,\n"
            "TWO_REAL_ROOTS     = 2,\n"
            "NO_SOLUTION        = 3,\n"
            "LINEAL_ROOT        = 4,\n"
            "INFINITE_SOLUTIONS = 5,\n"
            "ERROR              = 6.\n"
            "Example line: 1 -2 1 1 1\n"
            "Do you want to continue? [y/n].\n", TESTS_FILE_DEFAULT_NAME);

    int ans = 0;

    skip_spaces();

    if ((ans = getchar()) == EOF)
    {
        printf("EOF FOUND! SHUTTING DOWN!\n");

        GLOBAL_CURRENT_RETURN_CODE = EOF_FOUND;

        return;
    }
    clear_buf();

    if (ans == 'n')
    {
        printf("Goodbye!\n");
        return;
    }

    FILE* file_inp = fopen(test_file_name, "r");

    if (file_inp == NULL)
    {
        printf("ERROR: Can't open file! Please restart to try again.\n");
        return;
    }

    int test_num = 1; //номер теста, только для вывода; в конце в нем хранится кол-во тестов + 1
    int passed = 0, failed = 0, skipped = 0;

    double test_a = NAN, test_b = NAN, test_c = NAN, test_x1 = NAN, test_x2 = NAN;
    int test_sol_code = 0;
    while (fscanf(file_inp, "%lg %lg %lg %d", &test_a, &test_b, &test_c, &test_sol_code) == 4)
    {
        //если test_sol_code предполагает что корни должны быть, то нужное число читаем,
        //и отдаем в run_one_test(), а потом разберемся, совпадает ли

        switch (test_sol_code)
        {
            case LINEAL_ROOT:
            case ONE_REAL_ROOT:
                if (fscanf(file_inp, "%lg", &test_x1) != 1)
                {
                    printf("This test implies one root, but I can't find a real number in the line:\n");
                    echo_line(file_inp, stdin);
                    printf("\nSkipping test...\n");
                    continue;
                }

                break;
            case TWO_REAL_ROOTS:
                if (fscanf(file_inp, "%lg %lg", &test_x1, &test_x2) != 2)
                {
                    printf("This test implies two roots, but I can't find two real numbers in the line:\n");
                    echo_line(file_inp, stdin);
                    printf("\nSkipping test...\n");
                    continue;
                }

                break;
            default:
                ;
                break;
        }

        //теперь корни если должны были быть прочитаны, то находятся в test_x1 и test_x2

        TestRes testRes = run_one_test( {{test_a, test_b, test_c}, {(SolveRes) test_sol_code, test_x1, test_x2}}, test_num++);

        switch (testRes)
        {
            case PASSED:
                passed+=1;
                break;
            case FAILED:
                failed+=1;
                break;
            case SKIPPED:
                skipped+=1;
                break;
            default:
                assert(0 && "Unsupported case in switch (testRes)!");
                break;
        }

    }

    if ( fgetc(file_inp) == EOF )
    {
        printf("End of file reached, so work is done. Number of tests...\n"
        "passed: %d\nfailed: %d\nskipped: %d\ntotal: %d\nGoodbye!\n", passed, failed, skipped, test_num-1);

        fclose(file_inp);
        return;
    }
    else
    {
        printf("The next line can't be read:\n");
        echo_line(file_inp, stdout);
        printf("\n");

        if (test_num > 1) printf("Number of tests before this unreadable line...\n"
        "passed: %d\nfailed: %d\nskipped: %d\ntotal: %d\nGoodbye!\n", passed, failed, skipped, test_num-1);

        fclose(file_inp);

        printf("Shutting down. Goodbye!\n");
    }
}

static TestRes run_one_test(const TestParams params, const int test_num)
{
    print_stars(STARS_STRIP_WIDTH);
    printf("\nTest %d:\n", test_num);

    if ( ( !isfinite(params.test_coeffs.a) || !isfinite(params.test_coeffs.b) || !isfinite(params.test_coeffs.c) ))
    {
        printf("Sorry, coefficient(s) is/are out of supported range. "
        "Skipping this test...\n");

        print_stars(STARS_STRIP_WIDTH);
        printf("\n");
        return SKIPPED;
    }

    //коэффициенты норм

    if (params.test_sol.res < SOLVE_RES_MIN_CODE || params.test_sol.res > SOLVE_RES_MAX_CODE)
    {
        printf("Wrong solution code: %d! Skipping this test...\n", params.test_sol.res);

        print_stars(STARS_STRIP_WIDTH);
        printf("\n");
        return SKIPPED;
    }

    //solution code нормальный

    //printf("Test params: %lg %lg %lg\n", params.test_a, params.test_b, params.test_c);

    SolutionSquare fact_sol = solve_square( { params.test_coeffs.a, params.test_coeffs.b, params.test_coeffs.c } );

    if ( (int) fact_sol.res != params.test_sol.res)
    {

        printf("Solution codes don't match! Test solution code: %d, "
        "in fact solution code: %d. Test FAILED!\n", params.test_sol.res, (int) fact_sol.res);
        printf("Test params: %lg %lg %lg\n", params.test_coeffs.a, params.test_coeffs.b, params.test_coeffs.c);

        print_stars(STARS_STRIP_WIDTH);
        printf("\n");
        return FAILED;
    }

    //solution code верный
    //printf("Solution codes match: %d!\n", params.test_sol_code);

    TestRes testRes = SKIPPED;

    switch (params.test_sol.res)
    {
        case ZERO_REAL_ROOTS:
        case NO_SOLUTION:
        case INFINITE_SOLUTIONS:
        case ERROR_OVRFL_ROOTS:
        case ERROR_OVRFL_DISCR:
            printf("Test PASSED!\n");

            testRes = PASSED;

            break;
        case ONE_REAL_ROOT:
        case LINEAL_ROOT:
            if (are_dbls_equal(fact_sol.x1, params.test_sol.x1))
            {
                printf("Test PASSED!\n");

                testRes = PASSED;
            }
            else
            {
                printf("Test and fact roots DON'T match: %lg anf %lg. Test FAILED!\n", params.test_sol.x1, fact_sol.x1);
                printf("Test params: %lg %lg %lg\n", params.test_coeffs.a, params.test_coeffs.b, params.test_coeffs.c);

                testRes = FAILED;
            }
            break;
        case TWO_REAL_ROOTS:
            if  ( (are_dbls_equal(fact_sol.x1, params.test_sol.x1)
                   && are_dbls_equal(fact_sol.x2, params.test_sol.x2))
               || (are_dbls_equal(fact_sol.x1, params.test_sol.x2)
                   && are_dbls_equal(fact_sol.x2, params.test_sol.x1))
                )
            {
                printf("Test PASSED!\n");

                testRes = PASSED;
            }
            else
            {
                printf("Test and fact roots DON'T match! x1: "
                "%lg and %lg, x2: %lg and %lg. "
                "Test FAILED!\n", params.test_sol.x1, fact_sol.x1, params.test_sol.x2, fact_sol.x2);
                printf("Test params: %lg %lg %lg\n", params.test_coeffs.a, params.test_coeffs.b, params.test_coeffs.c);

                testRes = FAILED;
            }
            break;
        default:
            assert(0 && "unreachable default case in switch reached!"); //недопустимый case
            break;
    }

    print_stars(STARS_STRIP_WIDTH);
    printf("\n");

    return testRes;
}
