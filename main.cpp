/*
SQUARE SOLVER, также известная как квадратка
Примечания:
Не получилось сделать поддержку long double, компилятор не знает о %Lf
(пишу в кодблоксе от Деда)
Вместо символа новой строки как будто получаю символ возврата каретки, это
повлияло на clear_buf()
ПОКА ЧТО НЕТУ ПРОВЕРКИ НА ПЕРЕПОЛНЕНИЕ ВО ВРЕМЯ ВЫЧИСЛЕНИЙ
Заметки:
*/

#include <stdio.h>
#include <ctype.h>
#include <float.h>
#include <math.h>

enum SOLVE_RES
{
    ZERO_REAL_ROOTS = 0,
    ONE_REAL_ROOT,
    TWO_REAL_ROOTS,
    NO_SOLUTION,
    LINEAL_ROOT,
    INFINITE_SOLUTIONS,
    ERROR,
};

enum SOLVE_RES solve(double a, double b, double c, double *p_x1, double *p_x2);
//получает коэфф и записывает корни в x1 и x2, возвращает кол-во корней,
//то есть при возврате 1 x1=x2, при 0 в x1 и x2 неопределенное значение,
//возврат -1 означает ошибку

void get_input(double *p_a, double *p_b, double *p_c);
//получает коэффициенты, старается добиться правильного ввода

void clear_buf(void);
//старается "съесть" строчку до конца, включая символ конца строки, из стандартного ввоода

int main(void)
{
    printf("Hello fellow engineers! Enter any letter except t to begin.");
    printf("If you want to run tests, please enter letter t.\n");

    int ans = getchar();
    clear_buf();

    double a, b, c;
    if (ans == 't')
    {
        printf("Tests are not supported yet. :-(. Let's pretend you entered a different letter.\n");
        //*вызов функции которой пока нет*
        //return 0; //чтобы завершить программу
        //break; !!!ВРЕМЕННО
    }

    get_input(&a, &b, &c);

    double x1, x2;
    enum SOLVE_RES res = solve(a, b, c, &x1, &x2);

    switch (res)
    {
        case TWO_REAL_ROOTS:
            printf("Here are two roots: %lg %lg\n", x1, x2);
            break;
        case ONE_REAL_ROOT:
            printf("Here are two identical roots: %lg %lg\n", x1, x2);
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
            printf("It is not ax^2+bx+c=0, it is bx+c=0. x = -c/b. x = %lg.\n", x1);
            break;
        case INFINITE_SOLUTIONS:
            printf("Well, any real number is a solution. But a = b = c = 0 is too trivial, you know.\n");
            break;
        default:
            printf("Somehow function \"solve\" returned unsupported value...\n");
    }

    return 0;
}

enum SOLVE_RES solve(double a, double b, double c, double *p_x1, double *p_x2)
{
    /*
    Примечание: похоже что math.h работает только с double, поэтому нет смысла пытаться
    считать дискриминант как long double?
    */

    //особые случаи
    if ( fabs(a) < DBL_EPSILON ) //a == 0
    {
        if ( fabs(b) < DBL_EPSILON ) //b == 0
        {
            if ( fabs(c) < DBL_EPSILON )//c == 0
            {
                return INFINITE_SOLUTIONS;
            }
            else //c != 0
            {
                return NO_SOLUTION;
            }
        }
        else //b != 0
        {
            *p_x1 = -c/b;
            return LINEAL_ROOT;
        }
    }

    //обычные случаи, a != 0
    double discriminant = b*b - 4.0*a*c;

    if ( discriminant > 0.0 )//D > 0
    {
        *p_x1 = (-b + sqrt(discriminant)) / (2.0*a);
        *p_x2 = (-b - sqrt(discriminant)) / (2.0*a);
        return TWO_REAL_ROOTS;
    }
    else if ( fabs(discriminant) < DBL_EPSILON )//D == 0
    {
        *p_x1 = -b / (2.0*a);
        *p_x2 = *p_x1;
        return ONE_REAL_ROOT;
    }
    else if (discriminant < 0.0)//D < 0
    {
        return ZERO_REAL_ROOTS;
    }
    return ERROR;
}

void get_input(double *p_a, double *p_b, double *p_c)
{
    /*
    Получает три long double числа из входного потока (введённые через пробел)
    и помещает их в аргументы-указатели. Старается добиться от пользователя ввода корректных чисел.
    */

    while( 1 ){ //выход из цикла через return в случае нужного ввода!!!
        printf( "Enter real coefficients a, b, c in order "
                "to get ax^2 + bx + c = 0 solved, for example: \"1.45 5.19 12\".\n");

        while (scanf("%lf %lf %lf", p_a, p_b, p_c) != 3)
        {
            printf( "Sorry, I don't see here three real numbers separated by a space. "
                    "Please, try again.\n");
            clear_buf();
        }

        if ( fabs(*p_a) >= DBL_MAX or fabs(*p_b) >= DBL_MAX or fabs(*p_c) >= DBL_MAX )
        {
            printf( "Sorry, number(s) is/are out of supported range. Please enter "
                    "something with smaller absolute value.\n");
            continue;
        }

        printf( "You entered: %lg %lg %lg. Is it right?"
                " Enter letter y if yes or anything else otherwise.\n", *p_a, *p_b, *p_c);
        clear_buf();
        int c = getchar();
        clear_buf();
        if (c == 'y')
        {
            printf("Great!\n");
            return;
        }
    }
}

void clear_buf(void)
{
    /*
    "Съедает" ввод, пока не встретит непечатаемый символ.
    По какой-то причине при вводе данных на месте символа новой строки стоял
    символ возврата каретки, поэтому именно проверка на непечатаемый символ, а не
    на символ новой строки.
    */
    while ( isprint(getchar()) ) continue;
    return;
}
