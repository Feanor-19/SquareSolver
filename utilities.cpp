#include "utilities.h"

/*!
    @brief Used to return needed code from main when error happens in other functions.

    @details It is a global variable seen in every part of the program.
    It is usually returned by main().
    Its default value is MainReturnCode::NO_ERRORS, which is equal to 0.
    It must change only once, and immediately after that control must return to main().
*/
MainReturnCode GLOBAL_CURRENT_RETURN_CODE = NO_ERRORS;

void print_stars(int number)
{
    assert(number > 0);
    while (number--) putchar('*');
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

void skip_spaces(void)
{
    int c;
    while(isspace(c = getchar()))
        ;
    ungetc(c, stdin);
}
