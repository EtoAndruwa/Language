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

};

Node* get_recur_tree(Tree* tree_ptr, Lexer_struct* lexer_str_ptr,token* tok_arr_ptr);
Node* get_express(Tree* tree_ptr, token* tok_arr_ptr, size_t control_flag); 
Node* get_main(Tree* tree_ptr, token* tok_arr_ptr);
Node* get_decl_var(Tree* tree_ptr, token* tok_arr_ptr);
Node* get_assign(Tree* tree_ptr, token* tok_arr_ptr);
Node* rule_Pow(Tree* tree_ptr, token* tok_arr_ptr);
Node* rule_N(Tree* tree_ptr, token* tok_arr_ptr);
Node* rule_P(Tree* tree_ptr, token* tok_arr_ptr);
Node* rule_T(Tree* tree_ptr, token* tok_arr_ptr);
Node* rule_E(Tree* tree_ptr, token* tok_arr_ptr);
Node* rule_F(Tree* tree_ptr, token* tok_arr_ptr);
Node* rule_V(Tree* tree_ptr, token* tok_arr_ptr);
Node* get_func_decl(Tree* tree_ptr, token* tok_arr_ptr);
Node* get_return(Tree* tree_ptr, token* tok_arr_ptr);
Node* get_logic(Tree* tree_ptr, token* tok_arr_ptr);
Node* get_lib_funcs(Tree* tree_ptr, token* tok_arr_ptr);


#endif
