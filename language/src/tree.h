#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

enum error_codes_tree
{
    TREE_OK                      = 0,
    ERR_TREE_NULL_STR_PTR        = -1,
    ERR_TREE_TO_CALLOC_NODE      = -2,
    ERR_VAL_VAR_EMPT_HAS_CHILD   = -4,
};


typedef union Node_data
{
    int     op_number;           /// \brief The number of operator
    double  node_value;          /// \brief The value of the constant
    char    text[21]; /// \brief The text of the node (for variables)
}Node_data;

enum node_type_tree
{
    ERROR          = -1,
    EMPTY          = 0,
    OP_HEAD        = 1, //
    OP             = 2,

    VAL_HEAD       = 3, //
    VAL            = 4, 

    VAR_HEAD       = 5, //
    VAR            = 6,

    DECL_VAR_HEAD  = 7, //

    DECL_FUNC_HEAD = 8, //
    FUNC_HEAD      = 17,
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
int ctor_tree(Tree* tree_str_ptr);
void dtor_tree(Tree* tree_str_ptr);
void dtor_childs(Node* node_ptr);
Node* create_node(Tree* tree_ptr, double node_value, int node_type = EMPTY, char* text = nullptr, Node* left_child = nullptr, Node* right_child = nullptr);

#endif
