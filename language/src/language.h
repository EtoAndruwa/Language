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
static const char* FILE_ASM_NAME = "asm_code.asm";    /// \brief Defines the name of assembly file

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
    FUNC_SCANF         = 3,
    FUNC_PRINTF        = 4,
    DECL_FUNC_ARGS     = 5, // for decl vars as args of funcs
    NOT_FUNC_ARGS      = 6, // for common var x = 10;
    IS_POSITIVE        = 7,
    IS_ZERO            = 8,
    IS_NEGATIVE        = 9,
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
    ERR_FRT_INVAL_LIB_FUNC_CALL = -25,
    ERR_FRT_INVAL_LOG_OPS_SEQ   = -26,
};

enum error_codes_back
{
    BACK_OK                     = 0,
    ERR_BCK_CALLOC_VARS         = -1,
    ERR_BCK_CALLOC_FUNCS        = -2,
    ERR_BCK_OPEN_ASM_FILE       = -3,
    ERR_BCK_CLOSE_ASM_FILE      = -4,
    ERR_BCK_CALLOC_DECL_NODS    = -5,
    ERR_BCK_FOUND_UNDECL_VAR    = -6,
    ERR_BCK_FOUND_NEW_OP        = -7,
    ERR_BCK_VAR_ASSIGN          = -8,
    ERR_BCK_NEW_TYPE_VAR_DECL   = -9,
    ERR_BCK_NEW_TYPE_DECL_FUNC  = -10,
    ERR_BCK_NEW_TYPE_SUB_EQ     = -11,
    ERR_BCK_TRANSLATE_MAIN      = -12,
    ERR_BCK_TRANSLATE_VAR_DECL  = -13,
    ERR_BCK_TRANSLATE_SUB_EQ    = -14,
    ERR_BCK_TRANSLATE_EXPR      = -15, 
    ERR_BCK_NEW_TYPE_EXPR       = -16,
    ERR_BCK_TRANSLATE_FUNC_DECL = -17,
    ERR_BCK_NEW_TYPE_FUNC_CALL  = -18,
    ERR_BCK_REALLOC_VARS        = -19,
    ERR_BCK_REALLOC_FUNCS       = -20,
    ERR_BCK_REALLOC_DECLS       = -21,
    ERR_BCK_FUNC_REDECL         = -22,
    ERR_BCK_INVAL_ARGS_SCANF    = -23,
    ERR_BCK_INVAL_ARGS_PRINTF   = -24,
    ERR_BCK_VAR_REDECL          = -25,
    ERR_BCK_INV_NUM_OF_ARGS     = -26,
    ERR_BKC_FUNC_ARGS_NOT_FOUND = -27,
    ERR_BCK_MAIN_CANNOT_BE_CLLD = -28,
    ERR_BKC_INV_RETURN_MAIN     = -29,
    ERR_BCK_INVAL_ARGS_SQRT     = -30,
    ERR_BCK_NEW_LOG_OP          = -31,
};

typedef struct var_info
{
    char var_text[MAX_LEN_TOK_TEXT];
    size_t var_ram_id = 0;
    char name_parent_func[MAX_LEN_TOK_TEXT];
}var_info;

typedef struct func_info
{
    char func_name[MAX_LEN_TOK_TEXT];
    size_t num_of_vars = 0;
}func_info;

typedef struct Backend_struct
{
    func_info* funcs  = nullptr;
    var_info* vars    = nullptr;
    Node** decl_nodes = nullptr;

    size_t num_of_decls = 5;
    size_t num_of_funcs = 5;
    size_t num_of_vars  = 5;

    size_t cur_ram_id   = 0;
    size_t cur_decl_id  = 0;
    size_t cur_var_id   = 0;
    size_t cur_func_id  = 0;
    size_t cur_flag_id  = 1;
    
    size_t main_node_id = 0;
    int error_code      = BACK_OK;
}Backend_struct;

/*####################################################################################################################################################################*/

// FRONTEND FUNCS

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

// BACKEND FUNCS

void print_funcs(Backend_struct* backend_str_ptr);
void print_vars(Backend_struct* backend_str_ptr);
int dtor_backend(Backend_struct* backend_str_ptr);
int ctor_backend(Backend_struct* backend_str_ptr);
int find_main_node(Backend_struct* backend_str_ptr, Node* node_ptr);
void print_decls(Backend_struct* backend_str_ptr);
int translate_expr(Backend_struct* backend_str_ptr, Node* node_ptr, FILE* asm_file_ptr, char* func_name, int jmp_for_break = -1);
int translate_var_decl(Backend_struct* backend_str_ptr, Node* node_ptr, FILE* asm_file_ptr, char* func_name, int decl_flag = NOT_FUNC_ARGS);
int create_asm(Backend_struct* backend_str_ptr);
int print_sub_eq(Backend_struct* backend_str_ptr, Node* node_ptr, FILE* asm_file_ptr, char* func_name);
int translate_var_assign(Backend_struct* backend_str_ptr, Node* node_ptr, FILE* asm_file_ptr, char* func_name);
int print_decl_funcs(Backend_struct* backend_str_ptr, Node* node_ptr, FILE* asm_file_ptr, char* func_name);
int print_call_func(Backend_struct* backend_str_ptr, Node* node_ptr, FILE* asm_file_ptr, char* func_name);
int realloc_vars(Backend_struct* backend_str_ptr);
int realloc_funcs(Backend_struct* backend_str_ptr);
int realloc_decls(Backend_struct* backend_str_ptr);
int print_logic(Backend_struct* backend_str_ptr, Node* node_ptr, FILE* asm_file_ptr, char* func_name, int jmp_for_break = -1);
int count_func_args(Node* node_ptr);
int print_lib_funcs(Backend_struct* backend_str_ptr, Node* node_ptr, FILE* asm_file_ptr, char* func_name);
int check_func_args(Backend_struct* backend_str_ptr, Node* node_ptr, int flag);
int check_is_positive(double value) ;

/*##################################################x##################################################################################################################*/

#endif
