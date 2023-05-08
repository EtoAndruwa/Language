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
                    
// #include "../../graphviz/src/graphviz.h"  // The graphviz lib's header with funcs
#include "DSL.h"
#include "lexer.h"

/*####################################################################################################################################################################*/

enum return_codes
{

};


enum error_codes_front
{
    FRONT_OK               = 0,
    ERR_FRT_NO_MAIN        = -1,
    ERR_FRT_MAIN_BRACK     = -2,
    ERR_FRT_NO_OPEN_SQR_BR = -3,
    ERR_FRT_NO_CLOS_SQR_BR = -4,
    ERR_FRT_NO_OPEN_BR     = -5,
    ERR_FRT_NO_CLOS_BR     = -6,
    ERR_FRT_NO_OPEN_FIG_BR = -7,
    ERR_FRT_NO_CLOS_FIG_BR = -8,
    ERR_FRT_NO_END_LINE    = -9,
};

enum error_codes_back
{

};

enum error_codes_tree
{
    TREE_OK                 = 0,
    ERR_TREE_NULL_STR_PTR   = -1,
    ERR_TREE_TO_CALLOC_NODE = -2,
    ERR_VAL_VAR_HAS_CHILD   = -4,
};


typedef union Node_data
{
    int     op_number;           /// \brief The number of operator
    double  node_value;          /// \brief The value of the constant
    char    text[MAX_LEN_TOK_TEXT]; /// \brief The text of the node (for variables)
}Node_data;


typedef union Tok_data
{
    int    int_val; /// \brief This type of data is used to store the value of operator/function in the node
    double flt_val; /// \brief This type of data is used to store the value of constant node
}Tok_data;

enum node_type_tree
{
    EMPTY          = 0,
    OP_HEAD        = 1, //
    OP             = 2,

    VAL_HEAD       = 3, //
    VAL            = 4, 

    VAR_HEAD       = 5, //
    VAR            = 6,

    DECL_VAR_HEAD  = 7, //

    DECL_FUNC_HEAD = 8, //
    FUNC_INFO      = 9, // 
    FUNC_ARGS      = 10, //
    FUNC_NAME      = 11, //
    FUNC_CALL      = 12, //

    EXPR_HEAD      = 13, //

    LOGIC_OP_HEAD  = 14,
    LOGIC_OP       = 15,

    MAIN           = 16,
};

typedef struct Node
{
    Node_data value;                 /// \brief The value of the node which is union
    char      type        = EMPTY;   /// \brief The type of the node
    Node*     left_child  = nullptr; /// \brief The pointer to the left child of the node
    Node*     right_child = nullptr; /// \brief The pointer to the right child of the node
}Node;

typedef struct Tree
{
    Node*   root           = nullptr;  /// \brief The pointer to the root node of the tree 
    size_t  num_of_toks    = 0;        /// \brief (OLD) The total number of tokens 
    size_t  cur_tok        = 0;        /// \brief (OLD) The current number of tok in the array of tokens
    size_t  error_code     = TREE_OK;  /// \brief The error code of the struct
}Tree;

/*####################################################################################################################################################################*/
int tree_ctor(Tree* tree_str_ptr);
void tree_dtor(Tree* tree_str_ptr);
void dtor_childs(Node* node_ptr);
Node* create_node(Tree* tree_ptr, double node_value, int node_type = EMPTY, char* text = nullptr, Node* left_child = nullptr, Node* right_child = nullptr);

#endif
