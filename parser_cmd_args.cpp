#include "parser_cmd_args.h"

CmdFlags parse_cmd_args(int argc, const char * argv[])
{

    CmdFlags flags = {0, 0, 0, ""};
    while (--argc) //пропускаем первый аргумент
    {
        if (strcmp(*(++argv), "-tests") == 0)
        {
            flags.tests = 1;
        }
        else if (flags.tests && (strcmp(*argv, "-cstm-test") == 0))
        {
            flags.cstm_test = 1;

            //сразу после должен быть относительный путь до файла
            strcpy(flags.test_file_name, *(++argv));
            argc-=1;
        }
        else
        {
            flags.unknown_flags = 1;
        }
    }

    return flags;
}
