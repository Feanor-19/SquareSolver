/*
SQUARE SOLVER, также известная как квадратка
Примечания:
(пишу в кодблоксе от Деда)
Заметки:
переделать файл с тестами на csv
разбиение на файлы
доки
*/

#include "parser_cmd_args.h"
#include "input_square_solver.h"
#include "linear_solver.h"
#include "square_solver.h"
#include "print_square_solution.h"
#include "tests.h"
#include "utilities.h"

int main(int argc, const char *argv[]) // argv[] = (* const argv)
{
    GLOBAL_CURRENT_RETURN_CODE = NO_ERRORS;

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

        skip_spaces();
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

