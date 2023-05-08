#ifndef DSL_H
#define DSL_H

/*####################################################################################################################################################################*/

#define GET_CUR_TOK()                    \
    size_t cur_tok = tree_ptr->cur_tok;  \
    tree_ptr->cur_tok++;                 \

/*####################################################################################################################################################################*/    

#define NUM_NODE(value)                   create_node(tree_ptr, value);
#define MUL_NODE(left_child, right_child) create_node(tree_ptr, Mul, IS_OP, nullptr, left_child, right_child);
#define ADD_NODE(left_child, right_child) create_node(tree_ptr, Add, IS_OP, nullptr, left_child, right_child);
#define SUB_NODE(left_child, right_child) create_node(tree_ptr, Sub, IS_OP, nullptr, left_child, right_child);
#define DIV_NODE(left_child, right_child) create_node(tree_ptr, Div, IS_OP, nullptr, left_child, right_child);
#define SIN_NODE(left_child)              create_node(tree_ptr, Sin, IS_FUNC, nullptr, left_child);
#define COS_NODE(left_child)              create_node(tree_ptr, Cos, IS_FUNC, nullptr, left_child);
#define SQRT_NODE(left_child)             create_node(tree_ptr, Sqrt, IS_FUNC, nullptr, left_child);
#define TAN_NODE(left_child)              create_node(tree_ptr, Tan, IS_FUNC, nullptr, left_child);
#define COT_NODE(left_child)              create_node(tree_ptr, Cot, IS_FUNC, nullptr, left_child);
#define ASIN_NODE(left_child)             create_node(tree_ptr, Asin, IS_FUNC, nullptr, left_child);
#define ACOS_NODE(left_child)             create_node(tree_ptr, Acos, IS_FUNC, nullptr, left_child);
#define LN_NODE(left_child)               create_node(tree_ptr, Log, IS_FUNC, nullptr, left_child);
#define POW_NODE(base, exp)               create_node(tree_ptr, Pow, IS_FUNC, nullptr, base, exp);
#define EXP_NODE(exp)                     create_node(tree_ptr, Exp, IS_FUNC, nullptr, exp);
#define VARIB_NODE(text)                  create_node(tree_ptr, 0, IS_VARIB,  text);
#define CNST_VARIB_NODE(text)             create_node(tree_ptr, 0, IS_CNST_VAR, text);

/*####################################################################################################################################################################*/

#define LEX_ERROR       lexer_str_ptr->error_code
#define LEX_TOKS        lexer_str_ptr->tok_arr_ptr
#define STRING          lexer_str_ptr->buff_ptr
#define POSITION        lexer_str_ptr->cur_pos_buff
#define LEX_CUR_TOK     lexer_str_ptr->cur_tok

#define TREE_CUR_TOK    tree_ptr->cur_tok
#define TREE_ERR        tree_ptr->error_code

#define FUNC_NAME                         __func__
#define NODE_LEFT_CHILD                   node_ptr->left_child
#define NODE_RIGHT_CHILD                  node_ptr->right_child

/*####################################################################################################################################################################*/

#endif
