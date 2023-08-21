/*
SQUARE SOLVER, также известная как квадратка
Примечания:
(пишу в кодблоксе от Деда)
Заметки:
*/

#include <stdio.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include <assert.h>

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

struct Coeffs
{
    double a;
    double b;
    double c;
};

struct Solution
{
    SolveRes res;
    double x1;
    double x2;
};

//получает коэфф и записывает корни в x1 и x2, возвращает enum SOLVE_RES
Solution solve(Coeffs coeffs);

//получает коэффициенты, старается добиться правильного ввода
Coeffs get_input(void);

//складывает a и b, результат сложения возвращает, а если по пути произошло переполнение,
//пишет в res НЕ 0, если не было переполнения - НЕ МЕНЯЕТ значение res
double add_dbl(double a, double b, int *res);

//умножает a и b, результат умножения возвращает, а если по пути произошло переполнение,
//пишет в res НЕ 0, если не было переполнения - НЕ МЕНЯЕТ значение res
double mul_dbl(double a, double b, int* res);

//делит a на b, результат деления возвращает, а если по пути произошло переполнение,
//пишет в res НЕ 0, если не было переполнения - НЕ МЕНЯЕТ значение res
double div_dbl(double a, double b, int* res);

//получает один символ, после этого "съедает" всё до конца строчки
int get_one_char_ans();

//старается "съесть" строчку до конца, включая символ конца строки, из стандартного ввода
void clear_buf(void);

int main(void)
{
    printf( "Hello fellow engineers! Enter any letter except t to begin."
            "If you want to run tests, please enter letter t.\n");

    int ans = get_one_char_ans();

    //double a = 0, b = 0, c = 0;

    if (ans == 't')
    {
        printf("Tests are not supported yet. :-(. Let's pretend you entered a different letter.\n");
        //*вызов функции которой пока нет*
        //return 0; //чтобы завершить программу
        //break; !!!ВРЕМЕННО
    }

    while (1)//выход через return
    {
        Coeffs coeffs = get_input();

        Solution solution = solve(coeffs);

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

Solution solve(Coeffs coeffs)
{
    /*
    Получает коэфф и записывает корни в x1 и x2, возвращает enum SOLVE_RES
    Примечание: похоже что math.h работает только с double, поэтому нет смысла пытаться
    считать дискриминант как long double?
    */

    int ovrfl = 0; //остлеживание переполнения при вычислениях

    //особые случаи
    if ( fabs(coeffs.a) < DBL_EPSILON ) //a == 0
    {
        if ( fabs(coeffs.b) < DBL_EPSILON ) //b == 0
        {
            if ( fabs(coeffs.c) < DBL_EPSILON )//c == 0
            {
                return {INFINITE_SOLUTIONS, 0, 0};
            }
            else //c != 0
            {
                return {NO_SOLUTION, 0, 0};
            }
        }
        else //b != 0
        {
            //*p_x1 = -c/b;
            double x1 = div_dbl(-coeffs.c, coeffs.b, &ovrfl);

            if (ovrfl)
            {
                printf("Overflow during computing the lineal root!\n");
                return {ERROR, 0, 0};
            }

            return {LINEAL_ROOT, x1, x1};
        }
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
        //*p_x1 = (-b + sqrt(discriminant)) / (2.0*a);
        double x1 = div_dbl( add_dbl( -coeffs.b, +sqrt(discriminant), &ovrfl ), mul_dbl(2.0, coeffs.a, &ovrfl), &ovrfl );

        //*p_x2 = (-b - sqrt(discriminant)) / (2.0*a);
        double x2 = div_dbl( add_dbl( -coeffs.b, -sqrt(discriminant), &ovrfl ), mul_dbl(2.0, coeffs.a, &ovrfl), &ovrfl );

        if (ovrfl)
        {
            printf("Overflow during computing roots!\n");
            return {ERROR, 0, 0};
        }

        return {TWO_REAL_ROOTS, x1, x2};
    }
    else if ( fabs(discriminant) < DBL_EPSILON )//D == 0
    {
        //*p_x1 = -b / (2.0*a);
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

Coeffs get_input(void)
{
    /*
    Получает три long double числа из входного потока (введённые через пробел)
    и помещает их в аргументы-указатели. Старается добиться от пользователя ввода корректных чисел.
    */
    double a = 0, b = 0, c = 0;

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

        if ( fabs(a) >= DBL_MAX || fabs(b) >= DBL_MAX || fabs(c) >= DBL_MAX )
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

            //В Прате написано, что нужно писать (C99)
            //(struct Coeffs) {a, b, c}
            //однако если с++ разрешает вообще не писать ключевое слово struct
            //то наверно и в таком вот литерале можно круглые скобки с их
            //содержимым не писать?
            return {a, b, c};
        }
    }
}

double add_dbl(double a, double b, int *res)
{
    /*
    Примечание:
    1) переполнение может быть только если
    оба слагаемых одного знака
    2) не собрано в один if для читаемости и логики
    */
    if ( (a > 0 && b > 0) || (a < 0 && b < 0) )
    {
        if (fabs(a) > DBL_MAX - fabs(b))
        {
            *res = 1;
        }
    }

    return a + b;
}

double mul_dbl(double a, double b, int* res)
{
    /*
    Примечание:
    1) только если оба множителя больше 1, может произойти переполнение
    2) не собрано в один if для читаемости и логики
    */

    if (a > 1 && b > 1)
    {
        if ( fabs(a) > DBL_MAX / fabs(b) )
        {
            *res = 1;
        }
    }

    return a * b;
}

double div_dbl(double a, double b, int* res)
{
    /*
    Примечание:
    1) только если делим на число < 1, может произойти переполнение
    2) не собрано в один if для читаемости и логики
    */

    if (b < 1)
    {
        if ( fabs(a) > DBL_MAX * fabs(b) )
        {
            *res = 1;
        }
    }

    return a / b;
}

int get_one_char_ans()
{
    int c = getchar();

    if (c == EOF)
    {
        printf("EOF FOUND! SHUTTING DOWN!\n");
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
