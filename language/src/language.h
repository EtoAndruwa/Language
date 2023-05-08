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

#include "tree.h"                         // The tree header with struct                        
#include "../../graphviz/src/graphviz.h"  // The graphviz lib's header with funcs
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

enum error_codes_tree
{
    TREE_OK                = 0,
    ERR_TO_CALLOC_ROOT     = 1,
    ERR_TO_CALLOC_NODE     = 2,
    ERR_TO_CALLOC_TREE     = 3,
    ERR_VAL_VAR_HAS_CHILD  = 4,
};


enum node_type_tree
{
    IS_OP       = 1,    /// \brief Operator
    IS_VAL      = 2,    /// \brief Value
    IS_VARIB    = 3,    /// \brief Variable
    IS_FUNC     = 4,    /// \brief Function
    IS_EXPRESS  = 5,    
    IS_MAIN     = 6,    
    IS_LOGIC    = 7,    
};

typedef struct Node
{
    Node_data value;                 /// \brief The value of the node which is union
    char      type        = IS_VAL;  /// \brief The type of the node
    Node*     left_child  = nullptr; /// \brief The pointer to the left child of the node
    Node*     right_child = nullptr; /// \brief The pointer to the right child of the node
}Node;

typedef struct Tree
{
    Node*   root           = nullptr;  /// \brief The pointer to the root node of the tree 
    char*   tree_buff      = nullptr;  /// \brief The pointer to the buffer with the tree equation
    FILE*   file_ptr       = nullptr;  /// \brief The pointer to the file with the tree equation

    size_t  num_of_toks    = 1;        /// \brief (OLD) The total number of tokens 
    size_t  cur_tok        = 0;        /// \brief (OLD) The current number of tok in the array of tokens
    size_t  error_code     = TREE_OK;  /// \brief The error code of the struct
}Tree;

/*####################################################################################################################################################################*/

// Tree* tree_ctor();
void tree_dtor(Tree* tree_ptr);
void dtor_childs(Node* node_ptr);
Node* create_node(Tree* tree_ptr, double node_value, int node_type = IS_VAL, char* text = nullptr, Node* left_child = nullptr, Node* right_child = nullptr);
void print_leaves(Node* node_ptr);
void print_preorder(Node* node_ptr);

#endif
