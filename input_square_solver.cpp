#include "input_square_solver.h"

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

        skip_spaces();

        while (scanf("%lf %lf %lf", &a, &b, &c) != 3)
        {
            printf( "Sorry, I don't see here three real numbers separated by a space. "
                    "Please, try again.\n");
            clear_buf();
        }
        clear_buf();

        if ( !isfinite(a) || !isfinite(b) || !isfinite(c) )
        {
            printf( "Sorry, number(s) is/are out of supported range. Please enter "
                    "something with smaller absolute value.\n");
            continue;
        }

        printf( "You entered: %lg %lg %lg. Is it right? [y/n]\n", a, b, c);

        int ans;
        skip_spaces();
        if ((ans = getchar()) == EOF)
        {
            printf("EOF FOUND! SHUTTING DOWN!\n");

            GLOBAL_CURRENT_RETURN_CODE = EOF_FOUND;

            return {NAN, NAN, NAN}; //поскорее возврат в main
        }
        clear_buf();

        if (ans == 'y')
        {
            printf("Great!\n");

            assert(isfinite(a) && "coeff a is inf");
            assert(isfinite(b) && "coeff b is inf");
            assert(isfinite(c) && "coeff c is inf");

            return {a, b, c};
        }
    }
}
