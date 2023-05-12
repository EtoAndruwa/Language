#ifndef LANGAUGE_H
#define LANGAUGE_H

/*####################################################################################################################################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

/*####################################################################################################################################################################*/

#define EPS 1e-7        // Used in comparisson of doubles
#define PI  3.14159265  // Used in calculations
const int TREE_POISON = 0xDEAD;
static const char* FILE_ASM_NAME     = "asm_code.asm";    /// \brief Defines the name of assembly file

/*####################################################################################################################################################################*/
                    
#include "../../graphviz/src/graphviz.h"  // The graphviz lib's header with funcs
#include "DSL.h"
#include "lexer.h"
#include "tree.h"

/*####################################################################################################################################################################*/

enum return_codes
{
    CALLED_BY_LOGIC_OP = 1,
    CALLED_BY_FUNC     = 2,
};


enum error_codes_front
{
    FRONT_OK                    = 0,
    ERR_FRT_NO_MAIN             = -1,
    ERR_FRT_MAIN_BRACK          = -2,
    ERR_FRT_NO_OPEN_SQR_BR      = -3,
    ERR_FRT_NO_CLOS_SQR_BR      = -4,
    ERR_FRT_NO_OPEN_BR          = -5,
    ERR_FRT_NO_CLOS_BR          = -6,
    ERR_FRT_NO_OPEN_FIG_BR      = -7,
    ERR_FRT_NO_CLOS_FIG_BR      = -8,
    ERR_FRT_NO_END_LINE         = -9,
    ERR_FRT_INV_VAR_DECL        = -10,
    ERR_FRT_INV_ASSIGNMENT      = -11,
    ERR_FRT_INV_ARGS_FUNC       = -12,
    ERR_FRT_INV_DECL_FUNC       = -13,
    ERR_FRT_MULTIPLE_DEF_MAIN   = -14,
    ERR_FRT_RECUR_RULE_E        = -15,
    ERR_FRT_RECUR_RULE_T        = -16,
    ERR_FRT_RECUR_RULE_P        = -17,
    ERR_FRT_RECUR_RULE_POW      = -18,
    ERR_FRT_NO_RETURN_FUNC      = -19,
    ERR_RFT_INVALID_SYNTAX      = -20,
    ERR_FRT_EMPTY_LOG_STATM     = -21,
    ERR_FRT_EMPTY_LOG_BODY      = -22,
    ERR_FRT_INVALID_LOG_STATM   = -23,
    ERR_FRT_NO_COMMA_SEPARATOR  = -24,
    ERR_FRT_IVAL_LIB_FUNC_CALL  = -25,
};

enum error_codes_back
{
    BACK_OK     = 0,
    ERR_BCK_CALLOC_VARS = -1,
    ERR_BCK_CALLOC_FUNCS = -2,
    ERR_BCK_OPEN_ASM_FILE  = -3,
    ERR_BCK_CLOSE_ASM_FILE = -4,
    ERR_BCK_CALLOC_DECL_NODS = -5,
};

typedef struct var_info
{
    char var_text[MAX_LEN_TOK_TEXT];
    size_t var_ram_id = 0;
}var_info;

typedef struct func_info
{
    char func_name[MAX_LEN_TOK_TEXT];
    size_t num_of_vars = 0;
}func_info;

typedef struct Backend_struct
{
    size_t cur_ram_id = 0;
    func_info* funcs = nullptr;
    var_info* vars = nullptr;
    Node** decl_nodes = nullptr;

    size_t num_of_decls = 10;
    size_t num_of_funcs = 10;
    size_t num_of_vars = 10;

    size_t cur_decl_id = 0;
    size_t cur_var_id = 0;
    size_t cur_func_id = 0;
    size_t main_node_id = 0;
    int error_code = BACK_OK;
}Backend_struct;

/*####################################################################################################################################################################*/

Node* get_recur_tree(Tree_struct* tree_str_ptr, Lexer_struct* lexer_str_ptr,token* tok_arr_ptr);
Node* get_express(Tree_struct* tree_str_ptr, token* tok_arr_ptr, size_t control_flag); 
Node* get_main(Tree_struct* tree_str_ptr, token* tok_arr_ptr);
Node* get_decl_var(Tree_struct* tree_str_ptr, token* tok_arr_ptr);
Node* get_assign(Tree_struct* tree_str_ptr, token* tok_arr_ptr);
Node* rule_Pow(Tree_struct* tree_str_ptr, token* tok_arr_ptr);
Node* rule_N(Tree_struct* tree_str_ptr, token* tok_arr_ptr);
Node* rule_P(Tree_struct* tree_str_ptr, token* tok_arr_ptr);
Node* rule_T(Tree_struct* tree_str_ptr, token* tok_arr_ptr);
Node* rule_E(Tree_struct* tree_str_ptr, token* tok_arr_ptr);
Node* rule_F(Tree_struct* tree_str_ptr, token* tok_arr_ptr);
Node* rule_V(Tree_struct* tree_str_ptr, token* tok_arr_ptr);
Node* get_func_decl(Tree_struct* tree_str_ptr, token* tok_arr_ptr);
Node* get_return(Tree_struct* tree_str_ptr, token* tok_arr_ptr);
Node* get_logic(Tree_struct* tree_str_ptr, token* tok_arr_ptr);
Node* get_lib_funcs(Tree_struct* tree_str_ptr, token* tok_arr_ptr);

/*####################################################################################################################################################################*/

void print_funcs(Backend_struct* backend_str_ptr);
void print_vars(Backend_struct* backend_str_ptr);
int dtor_backend(Backend_struct* backend_str_ptr);
int ctor_backend(Backend_struct* backend_str_ptr);
Node* find_main_node(Backend_struct* backend_str_ptr, Node* node_ptr);
void print_decls(Backend_struct* backend_str_ptr);
void translate_expr(Backend_struct* backend_str_ptr, Node* node_ptr, FILE* asm_file_ptr);
void translate_var_decl(Backend_struct* backend_str_ptr, Node* node_ptr, FILE* asm_file_ptr);
int create_asm(Backend_struct* backend_str_ptr);
void print_sub_eq(Backend_struct* backend_str_ptr, Node* node_ptr, FILE* asm_file_ptr);

/*####################################################################################################################################################################*/

#endif
