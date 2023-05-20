#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "../../graphviz/src/debugger.h"  
#include "DSL.h"

const int LEX_POISON = 0xDEAD; 
const int MAX_LEN_TOK_TEXT = 21;

enum token_types
{
    #define DEF_OP(name, code, ...) name = code,
    #define DEF_FUNC(fname, fcode, ...) fname = fcode,
    #define DEF_LOGIC(lname, lcode, ...) lname = lcode,
    #define DEF_BRACK(bname, bcode, ...) bname = bcode,
    #define DEF_KEY_W(wname, wcode, ...) wname = wcode,
    #define DEF_LOGIC_OP(l_op_name, l_op_code, ...) l_op_name = l_op_code,
    #include "def_cmd.h"
    #undef DEF_OP
    #undef DEF_FUNC
    #undef DEF_LOGIC
    #undef DEF_BRACK
    #undef DEF_KEY_W
    #undef DEF_LOGIC_OP
};

enum error_codes_lexer
{
    LEXER_OK                = 0,
    ERR_LEX_CALLOC_LEXER    = -1,
    ERR_LEX_CALLOC_TOK_ARR  = -2,
    ERR_LEX_EMPTY_FILE      = -3, 
    ERR_LEX_OPEN_INP_FILE   = -4,
    ERR_LEX_CLOSE_INP_FILE  = -5,
    ERR_LEX_CALLOC_BUFF     = -6,
    ERR_LEX_READ_INTO_BUFF  = -7,
    ERR_LEX_MAX_LEN_TOK     = -8,
    ERR_LEX_INV_WORD_NAME   = -9,
    ERR_LEX_NEW_TOK_TYPE    = -10,
    ERR_LEX_INVALID_OP      = -12,
    ERR_LEX_INVALID_VAL     = -13,
    ERR_LEX_NO_END_LINE     = -14,
    ERR_LEX_BIG_LENGTH_VAL  = -15, 
    ERR_LEX_REALLOC_TOKS    = -16,
};

typedef union value_type
{
    int   int_val;
    float flt_val;
};

typedef struct token
{
    size_t token_type;
    value_type token_value;
    char token_text[MAX_LEN_TOK_TEXT];
}token;

typedef struct Lexer_struct
{
    token* tok_arr_ptr  = nullptr;
    char* buff_ptr      = nullptr;  
    int buff_size       = 0;
    int cur_pos_buff    = 0;
    int num_of_toks     = 10;
    int cur_tok         = 0;
    int error_code      = LEXER_OK;
}Lexer_struct;

int ctor_lexer(Lexer_struct* lexer_str_ptr);
int dtor_lexer(Lexer_struct* lexer_str_ptr);
int get_into_buff(Lexer_struct* lexer_str_ptr, FILE* file_inp_ptr);
int get_size(Lexer_struct* lexer_str_ptr, FILE* file_int_ptr);
int get_string(Lexer_struct* lexer_str_ptr, char* file_name);
void print_toks(Lexer_struct* lexer_str_ptr);
int get_toks(Lexer_struct* lexer_str_ptr);
int get_word(Lexer_struct* lexer_str_ptr);
int get_op(Lexer_struct* lexer_str_ptr);
int get_val(Lexer_struct* lexer_str_ptr);
int check_op(char op_char);
int realloc_toks(Lexer_struct* lexer_str_ptr);

// int get_string(Tree* const tree_str_ptr, char* file_name);

#endif
