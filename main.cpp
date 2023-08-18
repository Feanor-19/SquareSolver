/*
SQUARE SOLVER, также известная как квадратка
Не получилось сделать поддержку long double, компилятор не знает о %Lf
пишу в кодблоксе от Деда

Заметки:
DBL_EPSILON
DBL_MAX
*/

#include <stdio.h>
#include <ctype.h>

void get_input(double *p_a, double *p_b, double *p_c);
//получает коэффициенты, старается добиться правильного ввода

void clear_buf(void);
//старается "съесть" строчку до конца, включая символ конца строки, из стандартного ввоода

int main(void)
{
    printf("Hello fellow engineers! Enter any letter except t to begin.");
    printf("If you want to run tests, please enter letter t.\n");

    int ans = getchar();
    //putchar(ans);
    //ans = getchar();
    //printf("%d", ans);
    clear_buf();

    double a, b, c;
    switch(ans) //на случай увеличения количества вариантов
    {
        case 't':
            printf("Tests are not supported yet. :-(. Let's pretend you entered a different letter.\n");
            //break; !!!ВРЕМЕННО
        default:
            get_input(&a, &b, &c);
            printf("%lf %lf %lf", a, b, c);
    }

    return 0;
}

void get_input(double *p_a, double *p_b, double *p_c)
{
    /*
    Получает три long double числа из входного потока (введённые через пробел)
    и помещает их в аргументы-указатели. Старается добиться от пользователя ввода корректных чисел.
    НЕ ВКЛЮЧАЕТ ПРОВЕРКУ НА ТО, ВХОДЯТ ЛИ ЧИСЛА В ДОПУСТИМЫЙ ДЛЯ ТИПА ДИАПАЗОН
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
    while ( isprint(getchar()) ) continue;
    return;
}
