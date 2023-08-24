#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <ctype.h>

struct CoeffsSquare
{
    double a;
    double b;
    double c;
};

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

//-----

enum MainReturnCode
{
    NO_ERRORS = 0,
    EOF_FOUND = 1,
};

//ниже глобальная переменная, в которой хранится нынешнее возвращаемое всей программой значение;
//оно может поменяться только один раз, и как только оно становится не NO_ERRORS, программа как можно скорее
//добирается до main (если не уже там) и возвращает данный код ошибки (все это сделано, чтобы избавиться от abort и exit)
extern MainReturnCode GLOBAL_CURRENT_RETURN_CODE;

#define DEF_CHECK_CURRENT_RETURN_CODE { if (GLOBAL_CURRENT_RETURN_CODE != NO_ERRORS) return GLOBAL_CURRENT_RETURN_CODE; }

//---

const int STARS_STRIP_WIDTH = 40;
const double DBL_PRECISION = DBL_EPSILON * 10;
const int MAX_CUSTOM_TEST_FILE_NAME = 100;

//---

void print_stars(int number);

int are_dbls_equal(double x, double y);

//проверка, является x нулём
int is_dbl_zero(const double x);

//старается "съесть" строчку до конца, включая символ конца строки, из стандартного ввода
void clear_buf(void);

//все символы до конца строки из from копируются в to
void echo_line(FILE* from, FILE* to);

//пропускает все пробельные символы в вводе
void skip_spaces(void);

#endif
