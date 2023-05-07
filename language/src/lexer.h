#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "../../graphviz/src/debugger.h"  
#include "DSL_lexer.h"

const int LEX_POISON = 0xDEAD; 
const int MAX_LEN_TOK_TEXT = 21;

enum token_types
{
    #define DEF_OP(name, code, ...) name = code,
    #define DEF_FUNC(fname, fcode, ...) fname = fcode,
    #define DEF_LOGIC(lname, lcode, ...) lname = lcode,
    #define DEF_BRACK(bname, bcode, ...) bname = bcode,
    #define DEF_KEY_W(wname, wcode, ...) wname = wcode,
    #include "def_cmd.h"
    #undef DEF_OP
    #undef DEF_FUNC
    #undef DEF_LOGIC
    #undef DEF_BRACK
    #undef DEF_KEY_W
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

};

enum return_codes_lexer
{
    LEX_RET_OK,
    OP_NOT_FOUND,
    OP_FOUND,

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

typedef struct lexer_struct
{
    token* tok_arr_ptr  = nullptr;
    char* buff_ptr      = nullptr;  
    int buff_size       = 0;
    int cur_pos_buff    = 0;
    int num_of_toks     = 10;
    int cur_tok         = 0;
    int error_code      = LEX_RET_OK;
}lexer_struct;

int ctor_lexer(lexer_struct* lexer_str_ptr);
int dtor_lexer(lexer_struct* lexer_str_ptr);
int get_into_buff(lexer_struct* lexer_str_ptr, FILE* file_inp_ptr);
int get_size(lexer_struct* lexer_str_ptr, FILE* file_int_ptr);
int get_string(lexer_struct* lexer_str_ptr, char* file_name);
void print_toks(lexer_struct* lexer_str_ptr);
int get_toks(lexer_struct* lexer_str_ptr);
int get_word(lexer_struct* lexer_str_ptr);



// int get_string(Tree* const tree_ptr, char* file_name);

#endif
