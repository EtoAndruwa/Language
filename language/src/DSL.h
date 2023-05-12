#ifndef DSL_H
#define DSL_H

/*####################################################################################################################################################################*/

#define GET_CUR_TOK()                    \
    size_t cur_tok = tree_ptr->cur_tok;  \
    tree_ptr->cur_tok++;                 \

/*####################################################################################################################################################################*/    

#define VAL_NODE(value)                    create_node(tree_ptr, 0, VAL_HEAD, nullptr, create_node(tree_ptr, value, VAL), nullptr);
#define VAR_NODE(text)                     create_node(tree_ptr, 0, VAR_HEAD, nullptr, create_node(tree_ptr, 0, VAR,  text), nullptr);
#define DECL_VAR_NODE(text)                create_node(tree_ptr, 0, DECL_VAR_HEAD, nullptr, create_node(tree_ptr, 0, VAR_HEAD, nullptr, create_node(tree_ptr, 0, VAR,  text), nullptr), nullptr);

#define IF_NODE(statement, if_body, else_body)  create_node(tree_ptr, 0, LOGIC_OP_HEAD, nullptr, create_node(tree_ptr, If, LOGIC_OP, nullptr, statement, ELSE_NODE(if_body, else_body)), nullptr);
#define FOR_NODE(left_child, right_child)       create_node(tree_ptr, 0, LOGIC_OP_HEAD, nullptr, create_node(tree_ptr, For, LOGIC_OP, nullptr, left_child, right_child), nullptr);
#define WHILE_NODE(statement, while_body)     create_node(tree_ptr, 0, LOGIC_OP_HEAD, nullptr, create_node(tree_ptr, While, LOGIC_OP, nullptr, statement, while_body), nullptr);
#define ELSE_NODE(if_body, else_body)           create_node(tree_ptr, Else, LOGIC_OP, nullptr, if_body, else_body)

#define MUL_NODE(left_child, right_child)  create_node(tree_ptr, 0, OP_HEAD, nullptr, create_node(tree_ptr, Mul, OP, nullptr, left_child, right_child), nullptr);
#define ADD_NODE(left_child, right_child)  create_node(tree_ptr, 0, OP_HEAD, nullptr, create_node(tree_ptr, Add, OP, nullptr, left_child, right_child), nullptr);
#define SUB_NODE(left_child, right_child)  create_node(tree_ptr, 0, OP_HEAD, nullptr, create_node(tree_ptr, Sub, OP, nullptr, left_child, right_child), nullptr);
#define DIV_NODE(left_child, right_child)  create_node(tree_ptr, 0, OP_HEAD, nullptr, create_node(tree_ptr, Div, OP, nullptr, left_child, right_child), nullptr);
#define POW_NODE(left_child, right_child)  create_node(tree_ptr, 0, OP_HEAD, nullptr, create_node(tree_ptr, Pow, OP, nullptr, left_child, right_child), nullptr);
#define EQ_NODE(left_child, right_child)   create_node(tree_ptr, 0, OP_HEAD, nullptr, create_node(tree_ptr, Eq, OP, nullptr, left_child, right_child), nullptr);

#define MAIN_NODE(left_child)              create_node(tree_ptr, 0, MAIN, nullptr, left_child, nullptr);
#define EMPTY_NODE()                       create_node(tree_ptr, 0, EMPTY, nullptr, nullptr, nullptr);
#define ERROR_NODE()                       create_node(tree_ptr, 0, ERROR, nullptr, nullptr, nullptr);
#define EXPR_NODE(left_child, right_child) create_node(tree_ptr, 0, EXPR_HEAD, nullptr, left_child, right_child);

#define FUNC_CALL_NODE(left_child)              create_node(tree_ptr, 0, FUNC_CALL, nullptr, left_child, nullptr);
#define FUNC_DECL_NODE(left_child)              create_node(tree_ptr, 0, DECL_FUNC_HEAD, nullptr, left_child, nullptr);
#define FUNC_HEAD_NODE(left_child, right_child) create_node(tree_ptr, 0, FUNC_HEAD, nullptr, left_child, right_child);
#define FUNC_NAME_NODE(text)                    create_node(tree_ptr, 0, FUNC_NAME, text, nullptr, nullptr);
#define FUNC_INFO_NODE(left_child, right_child) create_node(tree_ptr, 0, FUNC_INFO, nullptr, left_child, right_child);
#define FUNC_ARGS_NODE(left_child, right_child) create_node(tree_ptr, 0, FUNC_ARGS, nullptr, left_child, right_child);
#define RETURN_NODE(left_child)                 create_node(tree_ptr, 0, RETURN, nullptr, left_child, nullptr);

// #define POW_NODE(base, exp)               create_node(tree_ptr, Pow, FUNC_NAME, nullptr, base, exp);
// #define SIN_NODE(left_child)              create_node(tree_ptr, Sin, FUNC_NAME, nullptr, left_child);
// #define COS_NODE(left_child)              create_node(tree_ptr, Cos, FUNC_NAME, nullptr, left_child);
// #define SQRT_NODE(left_child)             create_node(tree_ptr, Sqrt, FUNC_NAME, nullptr, left_child);
// #define TAN_NODE(left_child)              create_node(tree_ptr, Tan, FUNC_NAME, nullptr, left_child);
// #define COT_NODE(left_child)              create_node(tree_ptr, Cot, FUNC_NAME, nullptr, left_child);
// #define ASIN_NODE(left_child)             create_node(tree_ptr, Asin, FUNC_NAME, nullptr, left_child);
// #define ACOS_NODE(left_child)             create_node(tree_ptr, Acos, FUNC_NAME, nullptr, left_child);
// #define LN_NODE(left_child)               create_node(tree_ptr, Log, FUNC_NAME, nullptr, left_child);
// #define POW_NODE(base, exp)               create_node(tree_ptr, Pow, FUNC_NAME, nullptr, base, exp);
// #define EXP_NODE(exp)                     create_node(tree_ptr, Exp, FUNC_NAME, nullptr, exp);

/*####################################################################################################################################################################*/

#define LEX_ERROR       lexer_str_ptr->error_code
#define LEX_TOKS        lexer_str_ptr->tok_arr_ptr
#define STRING          lexer_str_ptr->buff_ptr
#define POSITION        lexer_str_ptr->cur_pos_buff
#define LEX_CUR_TOK     lexer_str_ptr->cur_tok

#define TREE_CUR_TOK    tree_ptr->cur_tok
#define TREE_ERR        tree_ptr->error_code


#define NODE_LEFT_CHILD   node_ptr->left_child
#define NODE_RIGHT_CHILD  node_ptr->right_child

/*####################################################################################################################################################################*/

#endif
