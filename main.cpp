/*
SQUARE SOLVER, также известная как квадратка
Примечания:
(пишу в кодблоксе от Деда)
Заметки:
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include <assert.h>

const char *TESTS_FILE_NAME = "SquareSolverTests.txt";
const int STARS_STRIP_WIDTH = 40;

enum SolveRes
{
    ZERO_REAL_ROOTS = 0,
    ONE_REAL_ROOT,
    TWO_REAL_ROOTS,
    NO_SOLUTION,
    LINEAL_ROOT,
    INFINITE_SOLUTIONS,
    ERROR,
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

//получает коэфф квадратного уравнения и возвращает результат решения с корнями
SolutionSquare solve_square(const CoeffsSquare coeffs);

//получает коэфф линейного уравнения и возвращает результат решения с корнями
SolutionLinear solve_linear(const CoeffsLinear coeffs);

//читает тесты из файла, сообщает о результатах прохождения каждого теста
void run_tests(void);

//возвращает коэффициенты, старается добиться правильного ввода
CoeffsSquare get_input(void);

//складывает a и b, результат сложения возвращает, а если по пути произошло переполнение,
//пишет в res НЕ 0, если не было переполнения - НЕ МЕНЯЕТ значение res
double add_dbl(const double a, const double b, int * const res);

//умножает a и b, результат умножения возвращает, а если по пути произошло переполнение,
//пишет в res НЕ 0, если не было переполнения - НЕ МЕНЯЕТ значение res
double mul_dbl(const double a, const double b, int * const res);

//делит a на b, результат деления возвращает, а если по пути произошло переполнение,
//пишет в res НЕ 0, если не было переполнения - НЕ МЕНЯЕТ значение res
double div_dbl(const double a, const double b, int * const res);

//проверка, является x нулём
int is_dbl_zero(const double x);

//получает один символ, после этого "съедает" всё до конца строчки
int get_one_char_ans(void);

//старается "съесть" строчку до конца, включая символ конца строки, из стандартного ввода
void clear_buf(void);

int is_dbl_ok(double x);

void print_stars(int number);

int main(void)
{
    printf( "# Square Solver by Feanor19, created to solve square equations.\n"
            "Hello fellow engineers! Enter any letter except t to begin."
            " If you want to run tests, please enter letter t.\n");

    int ans = get_one_char_ans();

    if (ans == 't'){
        run_tests();
        return 0; //чтобы завершить программу
    }

    while (1)//выход через return
    {
        CoeffsSquare coeffs = get_input();

        SolutionSquare solution = solve_square(coeffs);

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
            case ERROR:
                printf("Well, some error occurred during solving the equation.\n");
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
                printf("Somehow function \"solve\" returned unsupported value...\n");
        }

        printf( "If you want to continue, enter any letter except q. "
                "Otherwise enter q to exit.\n");

        ans = get_one_char_ans();

        if (ans == 'q')
        {
            printf("Goodbye, fellow engineers!\n");
            return 0;
        }
    }
    return 0;
}

SolutionSquare solve_square(const CoeffsSquare coeffs)
{
    /*
    получает структуру с коэфф и возвращает структурой результат решения с корнями
    */

    assert(isfinite(coeffs.a) && isfinite(coeffs.b) && isfinite(coeffs.c));

    int ovrfl = 0; //остлеживание переполнения при вычислениях

    //особые случаи
    if ( fabs(coeffs.a) < DBL_EPSILON ) //a == 0
    {
        SolutionLinear sol = solve_linear( (CoeffsLinear) {coeffs.b, coeffs.c} );
        return { sol.res, sol.x, 0 };
    }

    //обычные случаи, a != 0

    //double discriminant = b*b - 4.0*a*c;
    double discriminant = add_dbl(mul_dbl(coeffs.b, coeffs.b, &ovrfl), -mul_dbl(4.0, mul_dbl(coeffs.a, coeffs.c, &ovrfl), &ovrfl), &ovrfl);

    if (ovrfl)
    {
        printf("Overflow during computing discriminant!\n");
        return {ERROR, 0, 0};
    }

    if ( discriminant > 0.0 )//D > 0
    {
        double d_sqrt = sqrt(discriminant);

        //x1 = (-b + sqrt(discriminant)) / (2.0*a);
        double x1 = div_dbl( add_dbl( -coeffs.b, +d_sqrt, &ovrfl ), mul_dbl(2.0, coeffs.a, &ovrfl), &ovrfl );

        //x2 = (-b - sqrt(discriminant)) / (2.0*a);
        double x2 = div_dbl( add_dbl( -coeffs.b, -d_sqrt, &ovrfl ), mul_dbl(2.0, coeffs.a, &ovrfl), &ovrfl );

        if (ovrfl)
        {
            printf("Overflow during computing roots!\n");
            return {ERROR, 0, 0};
        }

        return {TWO_REAL_ROOTS, x1, x2};
    }
    else if ( is_dbl_zero(discriminant) )//D == 0
    {
        //x1 = -b / (2.0*a);
        double x1 = div_dbl( -coeffs.b, mul_dbl(2.0, coeffs.a, &ovrfl), &ovrfl );
        double x2 = x1;

        if (ovrfl)
        {
            printf("Overflow during computing roots!\n");
            return {ERROR, 0, 0};
        }

        return {ONE_REAL_ROOT, x1, x2};
    }
    else if (discriminant < 0.0)//D < 0
    {
        return {ZERO_REAL_ROOTS, 0, 0};
    }

    //если все случаи учтены, программа не должна добраться до этой строчки
    assert(0);

    //по-хорошему строчка ниже никогда не должна выполняться, но иначе варнинг
    return {ERROR, 0, 0};
}

SolutionLinear solve_linear(const CoeffsLinear coeffs)
{
    int ovrfl = 0;

    if ( fabs(coeffs.a) < DBL_EPSILON ) //a == 0
    {
        if ( fabs(coeffs.b) < DBL_EPSILON )//b == 0
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
        //x1 = -b/a;
        double x1 = div_dbl(-coeffs.b, coeffs.a, &ovrfl);

        if (ovrfl)
        {
            printf("Overflow during computing the lineal root!\n");
            return {ERROR, 0};
        }

        return {LINEAL_ROOT, x1};
    }
}

void run_tests(void)
{
    printf( "Please create file with name \"%s\" "
            "in the same folder this program runs in to "
            "do automatic tests. Every line of the file should include "
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
            "If you are ready, enter any letter except q, or enter q to exit.\n", TESTS_FILE_NAME);

    int ans = get_one_char_ans();

    if (ans == 'q')
    {
        printf("Goodbye!\n");
        return;
    }

    FILE* file_inp = fopen(TESTS_FILE_NAME, "r");

    if (file_inp == NULL)
    {
        printf("ERROR: Can't open file! Please restart to try again.\n");
        return;
    }

    int counter = 1;
    double a = NAN, b = NAN, c = NAN, x1 = NAN, x2 = NAN;
    int sol_code = 0;
    while (fscanf(file_inp, "%lg %lg %lg %d", &a, &b, &c, &sol_code) == 4)
    {
        print_stars(STARS_STRIP_WIDTH);
        printf("\nTest %d:\n", counter++);



        print_stars(STARS_STRIP_WIDTH);
        printf("\n");
    }

    if ( fgetc(file_inp) == EOF )
    {
        printf("End of file reached, so work is done. Goodbye!\n");
        return;
    }
    else
    {
        printf("The next line can't be read:\n");

        int symbol;
        while ((symbol = fgetc(file_inp)) != '\n') putchar(symbol);

        printf("Shutting down. Goodbye!\n");
    }
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

        if ( !is_dbl_ok(a) || !is_dbl_ok(b) || !is_dbl_ok(c) )
        {
            printf( "Sorry, number(s) is/are out of supported range. Please enter "
                    "something with smaller absolute value.\n");
            continue;
        }

        printf( "You entered: %lg %lg %lg. Is it right? [y/n]\n", a, b, c);

        int ans = get_one_char_ans();

        if (ans == 'y')
        {
            printf("Great!\n");

            //isnormal проверяет что лежит обычное число, а не inf, nan и подобные
            //а ноль за нормальное не считает...
            assert(isfinite(a) && isfinite(b) && isfinite(c));

            return {a, b, c};
        }
    }
}

double add_dbl(const double a, const double b, int * const res)
{
    /*
    Примечание:
    1) переполнение может быть только если
    оба слагаемых одного знака
    2) не собрано в один if для читаемости и логики
    */

    assert(res!=NULL);

    if ( (a > 0 && b > 0) || (a < 0 && b < 0) )
    {
        if (fabs(a) > DBL_MAX - fabs(b))
        {
            *res = 1;
        }
    }

    return a + b;
}

double mul_dbl(const double a, const double b, int * const res)
{
    /*
    Примечание:
    1) только если оба множителя больше 1, может произойти переполнение
    2) не собрано в один if для читаемости и логики
    */

    assert(res!=NULL);

    if (a > 1 && b > 1)
    {
        if ( fabs(a) > DBL_MAX / fabs(b) )
        {
            *res = 1;
        }
    }

    return a * b;
}

double div_dbl(const double a, const double b, int * const res)
{
    /*
    Примечание:
    1) только если делим на число < 1, может произойти переполнение
    2) не собрано в один if для читаемости и логики
    */

    assert(res!=NULL);

    if (b < 1)
    {
        if ( fabs(a) > DBL_MAX * fabs(b) )
        {
            *res = 1;
        }
    }

    return a / b;
}

int is_dbl_zero(const double x)
{
    return fabs(x) < DBL_EPSILON;
}

int get_one_char_ans(void)
{
    int c = getchar();

    if (c == EOF)
    {
        printf("EOF FOUND! SHUTTING DOWN!\n");
        abort();
    }

    clear_buf();

    return c;
}

void clear_buf(void)
{
    /*
    "Съедает" ввод, пока не встретит непечатаемый символ.
    */
    while ( getchar() != '\n' ) continue;
    return;
}

int is_dbl_ok(double x)
{
    return fabs(x) < DBL_MAX;
}

void print_stars(int number)
{
    assert(number > 0);
    while (number-- > 0) putchar('*');
}
