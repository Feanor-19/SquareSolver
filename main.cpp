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

int solve(double a, double b, double c, double *p_x1, double *p_x2);
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
    int res = solve(a, b, c, &x1, &x2);

    switch (res)
    {
        case 2:
            printf("Here are two roots: %lg %lg\n", x1, x2);
            break;
        case 1:
            printf("Here are two identical roots: %lg %lg\n", x1, x2);
            break;
        case 0:
            printf("There are no real roots... And I don't have imagination "
            "to compute complex ones.\n");
            break;
        case -1:
            printf("Well, some error occurred during solving the equation.\n");
            break;
        default:
            printf("Somehow method \"solve\" returned unsupported value...\n");
    }

    return 0;
}

int solve(double a, double b, double c, double *p_x1, double *p_x2)
{
    /*
    Примечание: похоже что math.h работает только
    */
    double discriminant = b*b - 4.0*a*c;

    if ( discriminant > 0.0 )//D > 0
    {
        *p_x1 = (-b + sqrt(discriminant)) / (2.0*a);
        *p_x2 = (-b - sqrt(discriminant)) / (2.0*a);
        return 2;
    }
    else if ( fabs(discriminant) < DBL_EPSILON )//D == 0
    {
        *p_x1 = -b / (2.0*a);
        *p_x2 = *p_x1;
        return 1;
    }
    else if (discriminant < 0.0)//D < 0
    {
        return 0;
    }
    return -1;
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
