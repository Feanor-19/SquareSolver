#ifndef PARSER_CMD_ARGS_H
#define PARSER_CMD_ARGS_H

#include "utilities.h"
#include "string.h"

struct CmdFlags // предполагает расширение до нескольких флагов
{
    int tests;
    int cstm_test;
    int unknown_flags; // ставится в 1 если какой-то флаг не был распознан
    char test_file_name[MAX_CUSTOM_TEST_FILE_NAME];
};

//---

CmdFlags parse_cmd_args(int argc, const char * argv[]);

#endif
