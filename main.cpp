/*
SQUARE SOLVER, также известная как квадратка
Примечания:
(пишу в кодблоксе от Деда)
Заметки:
исправить чтобы можно было писать пробелы перед вводом
в inline проверку на глобальный код возврата
ввод тестов из своего файла
переделать файл с тестами на csv
разбиение на файлы
доки
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include <assert.h>
#include <string.h>

const char *TESTS_FILE_DEFAULT_NAME = "SquareSolverTests.txt";
const int MAX_CUSTOM_TEST_FILE_NAME = 100;
const int STARS_STRIP_WIDTH = 40;
const double DBL_PRECISION = DBL_EPSILON * 10;

enum MainReturnCode
{
    NO_ERRORS = 0,
    EOF_FOUND = 1,
};

//ниже глобальная переменная, в которой хранится нынешнее возвращаемое всей программой значение;
//оно может поменяться только один раз, и как только оно становится не NO_ERRORS, программа как можно скорее
//добирается до main (если не уже там) и возвращает данный код ошибки (все это сделано, чтобы избавиться от abort и exit)
MainReturnCode GLOBAL_CURRENT_RETURN_CODE = NO_ERRORS;

enum SolveRes
{
    ZERO_REAL_ROOTS     = 0,
    ONE_REAL_ROOT       = 1,
    TWO_REAL_ROOTS      = 2,
    NO_SOLUTION         = 3,
    LINEAL_ROOT         = 4,
    INFINITE_SOLUTIONS  = 5,
    ERROR_OVRFL_DISCR   = 6,
    ERROR_OVRFL_ROOTS   = 7,
};
const int SOLVE_RES_MIN_CODE = 0; //ДОЛЖНО ОБНОВЛЯТЬСЯ В СООТВЕТСТВИИ С enum SolveRes
const int SOLVE_RES_MAX_CODE = 6; //ДОЛЖНО ОБНОВЛЯТЬСЯ В СООТВЕТСТВИИ С enum SolveRes

enum TestRes
{
    PASSED  = 0,
    FAILED  = 1,
    SKIPPED = 2,
};

struct CoeffsSquare
{
    double a;
    double b;
    double c;
};

struct CoeffsLinear
{
    double a;
    double b;
};

struct SolutionSquare
{
    SolveRes res;
    double x1;
    double x2;
};

struct SolutionLinear
{
    SolveRes res;
    double x;
};

struct TestParams
{
    CoeffsSquare test_coeffs;
    SolutionSquare test_sol;
};

struct CmdFlags // предполагает расширение до нескольких флагов
{
    int tests;
    int cstm_test;
    int unknown_flags; // ставится в 1 если какой-то флаг не был распознан
    char test_file_name[MAX_CUSTOM_TEST_FILE_NAME];
};

//получает коэфф квадратного уравнения и возвращает результат решения с корнями
SolutionSquare solve_square(const CoeffsSquare coeffs);

//получает коэфф линейного уравнения и возвращает результат решения с корнями
SolutionLinear solve_linear(const CoeffsLinear coeffs);

void print_square_solution(SolutionSquare solution);

//читает тесты из файла, сообщает о результатах прохождения каждого теста
void run_tests(const char test_file_name[]);

//выполняет один тест, умеет печатать, но не умеет читать из файла!!!
TestRes run_one_test(const TestParams params, const int test_num);

//возвращает коэффициенты, старается добиться правильного ввода
CoeffsSquare get_input(void);

CmdFlags parse_cmd_args(int argc, const char * argv[]);

int are_dbls_equal(double x, double y);

//проверка, является x нулём
int is_dbl_zero(const double x);

//получает один символ, после этого "съедает" всё до конца строчки
int get_one_char_ans(void);

//старается "съесть" строчку до конца, включая символ конца строки, из стандартного ввода
void clear_buf(void);

//все символы до конца строки из from копируются в to
void echo_line(FILE* from, FILE* to);

#define DEF_CHECK_CURRENT_RETURN_CODE { if (GLOBAL_CURRENT_RETURN_CODE != NO_ERRORS) return GLOBAL_CURRENT_RETURN_CODE; }

void print_stars(int number);

int main(int argc, const char *argv[]) // argv[] = (* const argv)
{
    //парсинг командной строки
    CmdFlags flags = parse_cmd_args(argc, argv);

    if (flags.unknown_flags) printf("Unknown flags were skipped!\n");

    if (flags.tests)
    {
        if (flags.cstm_test)
        {
            printf("Custom test file name set: %s\n", flags.test_file_name);
            run_tests(flags.test_file_name);
        }
        else
        {
            run_tests(TESTS_FILE_DEFAULT_NAME);
        }
    }

    DEF_CHECK_CURRENT_RETURN_CODE

    //---

    printf( "# Square Solver by Feanor19, created to solve square equations.\n"
            "Hello fellow engineers! Follow instructions on the screen to use this program.\n");

    int ans = 0;

    while (1)//выход через return
    {
        CoeffsSquare coeffs = get_input();

        DEF_CHECK_CURRENT_RETURN_CODE

        SolutionSquare solution = solve_square(coeffs);

        print_square_solution(solution);

        printf( "Do you want to continue? [y/n]\n");

        if ((ans = getchar()) == EOF)
        {
            printf("EOF FOUND! SHUTTING DOWN!\n");
            return EOF_FOUND;
        }
        clear_buf();

        if (ans == 'n')
        {
            printf("Goodbye, fellow engineers!\n");
            return NO_ERRORS;
        }
    }

    assert(0 && "Infinite while ended not using return...");

    return NO_ERRORS;
}

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

TestRes run_one_test(const TestParams params, const int test_num)
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

CoeffsSquare get_input(void)
{
    /*
    Получает три long double числа из входного потока (введённые через пробел)
    и возвращает в виде структуры. Старается добиться от пользователя ввода корректных чисел.
    */
    double a = NAN, b = NAN, c = NAN;

    while( 1 ){ //выход из цикла через return в случае нужного ввода!!!
        printf( "Enter real coefficients a, b, c in order "
                "to get ax^2 + bx + c = 0 solved, for example: \"1.45 5.19 12\".\n");

        while (scanf("%lf %lf %lf", &a, &b, &c) != 3)
        {
            printf( "Sorry, I don't see here three real numbers separated by a space. "
                    "Please, try again.\n");
            clear_buf();
        }
        clear_buf();

        if ( !isfinite(a) || !isfinite(b) || !isfinite(c) )
        {
            printf( "Sorry, number(s) is/are out of supported range. Please enter "
                    "something with smaller absolute value.\n");
            continue;
        }

        printf( "You entered: %lg %lg %lg. Is it right? [y/n]\n", a, b, c);

        int ans;
        if ((ans = getchar()) == EOF)
        {
            printf("EOF FOUND! SHUTTING DOWN!\n");

            GLOBAL_CURRENT_RETURN_CODE = EOF_FOUND;

            return {NAN, NAN, NAN}; //поскорее возврат в main
        }
        clear_buf();

        if (ans == 'y')
        {
            printf("Great!\n");

            assert(isfinite(a) && "coeff a is inf");
            assert(isfinite(b) && "coeff b is inf");
            assert(isfinite(c) && "coeff c is inf");

            return {a, b, c};
        }
    }
}

CmdFlags parse_cmd_args(int argc, const char * argv[])
{

    CmdFlags flags = {0, 0, 0, ""};
    while (--argc) //пропускаем первый аргумент
    {
        if (strcmp(*(++argv), "-tests") == 0)
        {
            flags.tests = 1;
        }
        else if (flags.tests && (strcmp(*argv, "-cstm-test") == 0))
        {
            flags.cstm_test = 1;

            //сразу после должен быть относительный путь до файла
            strcpy(flags.test_file_name, *(++argv));
            argc-=1;
        }
        else
        {
            flags.unknown_flags = 1;
        }
    }

    return flags;
}


int are_dbls_equal(double x, double y)
{
    return is_dbl_zero(x - y);
}

int is_dbl_zero(const double x)
{
    return fabs(x) < DBL_PRECISION;
}

void clear_buf(void)
{
    /*
    "Съедает" ввод, пока не встретит непечатаемый символ.
    */
    while ( getchar() != '\n' )
        ;
    return;
}

void echo_line(FILE* from, FILE* to)
{
    int symbol = 0;
    while ((symbol = fgetc(from)) != '\n') fputc(symbol, to);
}

void print_stars(int number)
{
    assert(number > 0);
    while (number--) putchar('*');
}
