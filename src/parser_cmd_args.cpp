#include "parser_cmd_args.h"

int parse_cmd_args(int argc, const char * argv[], size_t n_flags, CmdLineFlag flags[])
{
    assert(argv != NULL);
    assert(flags != NULL);
    assert(n_flags > 0);

    int skipped_args = 0;

    //пропускаем первый аргумент
    argc-=1;
    argv++;

    while( argc-- )
    {
        int flag_found = 0;

        for (size_t ind = 0; ind < n_flags; ind++)
        {
            if ( flags[ind].flag == NULL ) continue;

            //printf("%s %s\n", *argv, flags[ind].flag);

            if ( strcmp(*argv, flags[ind].flag) == 0 )
            {
                //printf("!!! %s %s\n", *argv, flags[ind].flag);

                flag_found = 1;

                flags[ind].state = 1;

                if ( flags[ind].needs_add_arg )
                {
                    //printf("here %d\n", argc);
                    if (argc != 0)
                    {
                        ++argv;
                        argc--;
                        printf("add_arg: %d\n", ((flags[ind]).add_arg)[0]);
                        //printf()

                        assert((flags[ind]).add_arg != NULL);

                        strcpy((flags[ind]).add_arg, *argv); // НЕ РАБОТАЕТ
                    }
                    else
                    {
                        //свой текстовый файл не задан

                        ((flags[ind]).add_arg)[0] = '\0';
                    }

                    //printf("%s %s\n", flags[ind].flag, flags[ind].add_arg);
                }
                else
                {
                    ((flags[ind]).add_arg)[0] = '\0';
                }

                break; //не может аргумент равняться двум флагам сразу
            }
        }

        if (!flag_found) skipped_args++;

        ++argv;
    }

    return skipped_args;
}
