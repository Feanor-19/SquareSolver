#include <stdio.h>

#include <stdarg.h>
#include <assert.h>

enum class AdvAssertType
{
    INT         = 0,
    CHAR        = 1,
    DOUBLE      = 2,
    FLOAT       = 3,
    STRING      = 4,
    LONG_INT    = 5,
    //POINTER     = 7,
};

void advassert_(int condition, int nArgs,...);

#define ADV_ASSERT(COND, ...)  advassert_(COND, __VA_ARGS__); assert(COND)

int main() // временно для тестов
{
    ADV_ASSERT(0 && "!!!aaa", 2, AdvAssertType::INT, 19, AdvAssertType::DOUBLE, 5.56);
    return 0;
}

void advassert_(int condition, int nArgs,...)
{
    if (!condition)
    {
        nArgs*=2; // потому что передается не фактическое количество аргументов, а сколько нужно вывести

        va_list args;
        va_start(args, nArgs);

        printf("!!!ADV_ASSERT!!!\n");

        if (nArgs == 0) fprintf(stderr, "No arguments to print.\n");

        for (int n = nArgs; n > 0; n-=2)
        {
            AdvAssertType type = (AdvAssertType) va_arg(args, int); // 'AdvAssertType' is promoted to 'int' when passed through '...'
            switch(type)
            {
                case AdvAssertType::INT:
                    fprintf(stderr, "%d\n", va_arg(args, int));
                    break;
                case AdvAssertType::CHAR:
                    fprintf(stderr, "%c\n", (char) va_arg(args, int)); // 'char' is promoted to 'int' when passed through '...'
                    break;
                case AdvAssertType::DOUBLE:
                case AdvAssertType::FLOAT:      // 'float' is promoted to 'double' when passed through '...'
                    fprintf(stderr, "%f\n", va_arg(args, double));
                    break;
                case AdvAssertType::STRING:
                    fprintf(stderr, "%s\n", va_arg(args, char *));
                    break;
                case AdvAssertType::LONG_INT:
                    fprintf(stderr, "%li\n", va_arg(args, long int));
                    break;
                //case POINTER:
                //    fprintf(stderr, "%d\n", va_arg(args, int));
                //    break;
                default:
                    printf("ADV_ASSERT: Unknown AdvAssertType. Can't read arguments...\n");
                    break;

            }
        }

        va_end(args);
    }
}
