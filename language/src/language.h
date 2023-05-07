#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

/*####################################################################################################################################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

/*####################################################################################################################################################################*/

#define EPS 1e-7        // Used in comparisson of doubles
#define PI  3.14159265  // Used in calculations
static const size_t MAX_LEN_VARIB  = 21; // The maximum length of the variable text = 20 + 1 terminating symbol
static const char* LATEX_FILE_NAME = "Tree.tex";
static const char* LATEX_DIR_NAME  = "latex/";

/*####################################################################################################################################################################*/

#include "tree.h"                         // The tree header with struct
#include "DSL.h"                          // The DSL header with macros
#include "../../graphviz/src/graphviz.h"  // The graphviz lib's header with funcs

/*####################################################################################################################################################################*/

/**
 * @brief This enum contains the codes of returns of the functions
 */
enum return_codes
{
    NULL_PTR_NODE  = 1,
    NOT_ALL_DIGITS = 2,
    IS_INT         = 3,
    IS_FLOAT       = 4,
    VALID_TOK      = 5,
    INVALID_TOK    = 6,
    EXISTING_FUNC  = 7,
    NON_EXIST_FUNC = 8,
    IS_POSITIVE    = 9,
    IS_NEGATIVE    = 10,
    IS_ZERO        = 11,
    EQ_FOR_LATEX   = 12,
    EQ_FOR_PLOT    = 13,
};

/**
 * @brief This enum contains all error codes of the differetiator's part of project
 */
enum error_codes_diff
{
    ERR_CANNOT_OPEN_OUTPUT  = -1,
    ERR_CANNOT_CLOSE_OUTPUT = -2, 
    ERR_CANNOT_OPEN_INPUT   = -3,
    ERR_CANNOT_CLOSE_INPUT  = -4,
    ERR_INVALID_VAR_NUM     = -5,
    ERR_TO_CALLOC_VARS      = -6,
    ERR_INVALID_VAR_TEXT    = -7,
    ERR_TO_CALLOC_TOKS      = -8,
    ERR_INVALID_TOKEN       = -9,
    ERR_OPEN_PARSER_LOG     = -10,
    ERR_CLOSE_PARSER_LOG    = -11,
    ERR_NO_END_OF_LINE      = -12,
    ERR_NO_CLOSING_BRACKETS = -12,
    ERR_UNKNOWN_FUNC        = -13,
    ERR_NULL_PTR_NODE       = -14,
    ERR_EMPTY_INP_FILE      = -15,
    ERR_TO_REALLOC_TOKS     = -16,
    ERR_INVALID_ARGUMENT    = -17,
    ERR_NOT_A_SPACE         = -18,
    ERR_FOUND_DIFF_NUM_VARS = -19,
    ERR_UNKNOWN_OPERATOR    = -20,
    ERR_DIV_TO_ZERO         = -21,
    ERR_UNCERTAINTY         = -22,
    ERR_OPEN_LATEX_FILE     = -23,
    ERR_CLOSE_LATEX_FILE    = -24,
    ERR_PRINT_LATEX         = -25,
    ERR_INVALID_TYPE_NODE   = -26,
};


#endif
