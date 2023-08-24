#ifndef PARSER_CMD_ARGS_H
#define PARSER_CMD_ARGS_H

#include "utilities.h"
#include "string.h"

/*!
    @brief Contains all supported command line flags and some necessary fields.
*/
struct CmdFlags // предполагает расширение до нескольких флагов
{
    int tests;                         ///< flag: turns on tests mode
    int cstm_test;                     ///< flag: allows to specify custom file with tests, its name must be placed afterwards
    int unknown_flags;                 ///< sets to 1 if unknown flags are met
    char test_file_name[MAX_CUSTOM_TEST_FILE_NAME]; ///< if -cstm-test, here the custom name is stored
};

//---
/*!
    @brief Parse command line arguments.

    @param argc Like in main().
    @param argv Like in main().
*/
CmdFlags parse_cmd_args(int argc, const char * argv[]);

#endif