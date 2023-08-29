#include "parser_cmd_args.h"
#include "input_square_solver.h"
#include "linear_solver.h"
#include "square_solver.h"
#include "print_square_solution.h"
#include "tests.h"
#include "utilities.h"

static const char help_message[] = "No help message as for now :-(";
static CmdLineFlag flags[] =       {
                                        {"-h", 0, 0, ""},
                                        {"-t", 0, 1, ""}
                                   };
//не получилось сделать ни дефолтные значения...
static const size_t HELP = 0, TESTS = 1;


int main(int argc, const char *argv[])
{
    GLOBAL_CURRENT_RETURN_CODE = NO_ERRORS;

    if (argc > 1)
    {
        //парсинг командной строки
        int skipped_args = parse_cmd_args(  argc,
                                            argv,
                                            sizeof(flags)/sizeof(flags[0]),
                                            flags );

        if (skipped_args != 0) printf("%d unknown flag(s) was/were skipped!\n", skipped_args);

        if (flags[HELP].state)
        {
            printf("%s", help_message);
            return GLOBAL_CURRENT_RETURN_CODE;
        }

        //printf("state = %d\n", flags[TESTS].state);

        if (flags[TESTS].state)
        {
            if ( ((flags[TESTS]).add_arg)[0] != '\0' )
            {
                //add_arg не пустой, значит было указано имя тестового файла
                printf("Custom test file name set: %s\n", flags[TESTS].add_arg);
                run_tests(flags[TESTS].add_arg);
                return GLOBAL_CURRENT_RETURN_CODE;
            }
            else
            {
                run_tests(TESTS_FILE_DEFAULT_NAME);
                return GLOBAL_CURRENT_RETURN_CODE;
            }

            assert(0 && "Unreachable line!");
        }
    }

    DEF_CHECK_CURRENT_RETURN_CODE;

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

