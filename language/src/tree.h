#ifndef TREE_H
#define TREE_H 

/*####################################################################################################################################################################*/

const int TREE_POISON = 0xDEAD;

/**
 * @brief This union is used in order to store the number of arithmetical operator or the value of the node
 */
typedef union Node_data
{
    int     op_number;           /// \brief The number of operator
    double  node_value;          /// \brief The value of the constant
    char    text[MAX_LEN_VARIB]; /// \brief The text of the node (for variables)
}Node_data;

/**
 * @brief This unioin is used in order to store the value of nodes for differentiation using strtok
 */
typedef union Tok_data
{
    int    int_val; /// \brief This type of data is used to store the value of operator/function in the node
    double flt_val; /// \brief This type of data is used to store the value of constant node
}Tok_data;

/*####################################################################################################################################################################*/

/**
 * @brief This enum contains the erro codes of the tree functions
 */
enum error_codes
{
    TREE_OK                = 0,
    ERR_TO_CALLOC_ROOT     = 1,
    ERR_TO_CALLOC_NODE     = 2,
    ERR_TO_CALLOC_TREE     = 3,
    ERR_VAL_VAR_HAS_CHILD  = 4,
};

/**
 * @brief This enum contains the names and numbers of functions and operators
 */
// enum op_func_numbers 
// {
//     #define DEF_OP(name_op, code_op, ...) name_op = code_op,
//     #define DEF_FUNC(name_func, code_func, ...) name_func = code_func,
//     #define DEF_LOGIC(name_logic, code_logic, ...) name_logic = code_logic,
//     #include "def_cmd.h"
//     #undef DEF_OP
//     #undef DEF_FUNC
//     #undef DEF_LOGIC
// };

/**
 * @brief This enum stores the types of the nodes
 */
enum node_type
{
    IS_OP       = 1,    /// \brief Operator
    IS_VAL      = 2,    /// \brief Value
    IS_VARIB    = 3,    /// \brief Variable
    IS_FUNC     = 4,    /// \brief Function
    IS_EXPRESS  = 5,    /// \brief Constant variable
    IS_MAIN     = 6,    /// \brief Constant variable
    IS_LOGIC    = 6,    /// \brief Constant variable
};

/*####################################################################################################################################################################*/

/**
 * @brief This struct contains the information about the single token
 */
typedef struct Tokens
{
    Tok_data value;                /// \brief This value is reponsible for asm code of the token
    size_t   type = IS_VAL;        /// \brief This value is responsible for type of token ('cmd','reg', 'val', 'flg')
    char     text[MAX_LEN_VARIB];  /// \brief This value is responsible for storing the string value of the node (for variables)
}Tokens;

/**
 * @brief This struct contains the information about the single node
 */
typedef struct Node
{
    Node_data value;                 /// \brief The value of the node which is union
    char      type        = IS_VAL;  /// \brief The type of the node
    Node*     left_child  = nullptr; /// \brief The pointer to the left child of the node
    Node*     right_child = nullptr; /// \brief The pointer to the right child of the node
}Node;

/**
 * @brief This struct describes the single variable
 */
typedef struct Var_str
{
    char   var_text[MAX_LEN_VARIB];   /// \brief The text of the variable
    double var_value = 0;             /// \brief The value which will replace the exact variable 
}Var_str;

/*####################################################################################################################################################################*/

/**
 * @brief This struct contains all information about the tree
 */
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

Tree* tree_ctor();
void tree_dtor(Tree* tree_ptr);
void dtor_childs(Node* node_ptr);
Node* create_node(Tree* tree_ptr, double node_value, int node_type = IS_VAL, char* text = nullptr, Node* left_child = nullptr, Node* right_child = nullptr);
void print_leaves(Node* node_ptr);
void print_preorder(Node* node_ptr);

#endif
