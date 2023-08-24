#include "utilities.h"

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
