#ifndef DSL_H
#define DSL_H

/*####################################################################################################################################################################*/

#define GET_CUR_TOK()                    \
    size_t cur_tok = tree_str_ptr->cur_tok;  \
    tree_str_ptr->cur_tok++;                 \

/*####################################################################################################################################################################*/    

#define VAL_NODE(value)                    create_node(tree_str_ptr, 0, VAL_HEAD, nullptr, create_node(tree_str_ptr, value, VAL), nullptr);
#define VAR_NODE(text)                     create_node(tree_str_ptr, 0, VAR_HEAD, nullptr, create_node(tree_str_ptr, 0, VAR,  text), nullptr);
#define DECL_VAR_NODE(text)                create_node(tree_str_ptr, 0, DECL_VAR_HEAD, nullptr, create_node(tree_str_ptr, 0, VAR_HEAD, nullptr, create_node(tree_str_ptr, 0, VAR,  text), nullptr), nullptr);

#define IF_NODE(statement, if_body, else_body)  create_node(tree_str_ptr, 0, STATM_OP_HEAD, nullptr, create_node(tree_str_ptr, If, STATM_OP, nullptr, statement, ELSE_NODE(if_body, else_body)), nullptr);
#define FOR_NODE(statement, for_body)           create_node(tree_str_ptr, 0, STATM_OP_HEAD, nullptr, create_node(tree_str_ptr, For, STATM_OP, nullptr, statement, for_body), nullptr);
#define WHILE_NODE(statement, while_body)       create_node(tree_str_ptr, 0, STATM_OP_HEAD, nullptr, create_node(tree_str_ptr, While, STATM_OP, nullptr, statement, while_body), nullptr);
#define ELSE_NODE(if_body, else_body)           create_node(tree_str_ptr, Else, STATM_OP, nullptr, if_body, else_body)
#define BREAK_NODE()                            create_node(tree_str_ptr, Break, BREAK, nullptr, nullptr, nullptr);
#define LOGIC_OP_NODE(code)                     create_node(tree_str_ptr, 0, LOGIC_OP_HEAD, nullptr, create_node(tree_str_ptr, code, LOGIC_OP));

#define MUL_NODE(left_child, right_child)  create_node(tree_str_ptr, 0, OP_HEAD, nullptr, create_node(tree_str_ptr, Mul, OP, nullptr, left_child, right_child), nullptr);
#define ADD_NODE(left_child, right_child)  create_node(tree_str_ptr, 0, OP_HEAD, nullptr, create_node(tree_str_ptr, Add, OP, nullptr, left_child, right_child), nullptr);
#define SUB_NODE(left_child, right_child)  create_node(tree_str_ptr, 0, OP_HEAD, nullptr, create_node(tree_str_ptr, Sub, OP, nullptr, left_child, right_child), nullptr);
#define DIV_NODE(left_child, right_child)  create_node(tree_str_ptr, 0, OP_HEAD, nullptr, create_node(tree_str_ptr, Div, OP, nullptr, left_child, right_child), nullptr);
#define POW_NODE(left_child, right_child)  create_node(tree_str_ptr, 0, OP_HEAD, nullptr, create_node(tree_str_ptr, Pow, OP, nullptr, left_child, right_child), nullptr);
#define EQ_NODE(left_child, right_child)   create_node(tree_str_ptr, 0, OP_HEAD, nullptr, create_node(tree_str_ptr, Eq, OP, nullptr, left_child, right_child), nullptr);

#define MAIN_NODE(left_child)              create_node(tree_str_ptr, 0, MAIN, nullptr, left_child, nullptr);
#define EMPTY_NODE()                       create_node(tree_str_ptr, 0, EMPTY, nullptr, nullptr, nullptr);
#define ERROR_NODE()                       create_node(tree_str_ptr, 0, ERROR, nullptr, nullptr, nullptr);
#define EXPR_NODE(left_child, right_child) create_node(tree_str_ptr, 0, EXPR_HEAD, nullptr, left_child, right_child)

#define FUNC_CALL_NODE(left_child)              create_node(tree_str_ptr, 0, FUNC_CALL, nullptr, left_child, nullptr);
#define FUNC_DECL_NODE(left_child)              create_node(tree_str_ptr, 0, DECL_FUNC_HEAD, nullptr, left_child, nullptr);
#define FUNC_HEAD_NODE(left_child, right_child) create_node(tree_str_ptr, 0, FUNC_HEAD, nullptr, left_child, right_child);
#define FUNC_NAME_NODE(text)                    create_node(tree_str_ptr, 0, FUNC_NAME, text, nullptr, nullptr);
#define FUNC_INFO_NODE(left_child, right_child) create_node(tree_str_ptr, 0, FUNC_INFO, nullptr, left_child, right_child);
#define FUNC_ARGS_NODE(left_child, right_child) create_node(tree_str_ptr, 0, FUNC_ARGS, nullptr, left_child, right_child);
#define RETURN_NODE(left_child)                 create_node(tree_str_ptr, 0, RETURN, nullptr, left_child, nullptr);

// #define POW_NODE(base, exp)               create_node(tree_str_ptr, Pow, FUNC_NAME, nullptr, base, exp);
// #define SIN_NODE(left_child)              create_node(tree_str_ptr, Sin, FUNC_NAME, nullptr, left_child);
// #define COS_NODE(left_child)              create_node(tree_str_ptr, Cos, FUNC_NAME, nullptr, left_child);
// #define SQRT_NODE(left_child)             create_node(tree_str_ptr, Sqrt, FUNC_NAME, nullptr, left_child);
// #define TAN_NODE(left_child)              create_node(tree_str_ptr, Tan, FUNC_NAME, nullptr, left_child);
// #define COT_NODE(left_child)              create_node(tree_str_ptr, Cot, FUNC_NAME, nullptr, left_child);
// #define ASIN_NODE(left_child)             create_node(tree_str_ptr, Asin, FUNC_NAME, nullptr, left_child);
// #define ACOS_NODE(left_child)             create_node(tree_str_ptr, Acos, FUNC_NAME, nullptr, left_child);
// #define LN_NODE(left_child)               create_node(tree_str_ptr, Log, FUNC_NAME, nullptr, left_child);
// #define POW_NODE(base, exp)               create_node(tree_str_ptr, Pow, FUNC_NAME, nullptr, base, exp);
// #define EXP_NODE(exp)                     create_node(tree_str_ptr, Exp, FUNC_NAME, nullptr, exp);

/*####################################################################################################################################################################*/

#define LEX_ERROR       lexer_str_ptr->error_code
#define LEX_TOKS        lexer_str_ptr->tok_arr_ptr
#define STRING          lexer_str_ptr->buff_ptr
#define POSITION        lexer_str_ptr->cur_pos_buff
#define LEX_CUR_TOK     lexer_str_ptr->cur_tok

#define TREE_CUR_TOK    tree_str_ptr->cur_tok
#define TREE_ERR        tree_str_ptr->error_code

#define NODE_LEFT_CHILD   node_ptr->left_child
#define NODE_RIGHT_CHILD  node_ptr->right_child

#define BACK_ERROR      backend_str_ptr->error_code
#define CUR_RAM_ID      backend_str_ptr->cur_ram_id

#define VARS_ARR        backend_str_ptr->vars
#define FUNCS_ARR       backend_str_ptr->funcs
#define DECL_NODES      backend_str_ptr->decl_nodes

#define CUR_VAR_ID      backend_str_ptr->cur_var_id
#define CUR_FUNC_ID     backend_str_ptr->cur_func_id
#define VAR_NUM         backend_str_ptr->num_of_vars
#define FUNC_NUM        backend_str_ptr->num_of_funcs
#define CUR_DECL_ID     backend_str_ptr->cur_decl_id
#define DECL_NUM        backend_str_ptr->num_of_decls
#define FLAG_ID         backend_str_ptr->cur_flag_id


/*####################################################################################################################################################################*/

// LANG CONFIG
// #define MAIN_LANG_DEF    "assalamaleikum"
// #define DECL_LANG_DEF    "kettik"
// #define VAR_LANG_DEF     "soz_berem_brat"
// #define RETURN_LANG_DEF  "jebersh"
// #define PRINTF_LANG_DEF  "korsish"
// #define SCANF_LANG_DEF   "ait_magan"
// #define IF_LANG_DEF      "eger"
// #define ELSE_LANG_DEF    "nemese"
// #define FOR_LANG_DEF     "esepshigi"
// #define WHILE_LANG_DEF   "bolgansha"
// #define BREAK_LANG_DEF   "toktay"
// #define SQRT_LANG_DEF    "sqrt"

#define MAIN_LANG_DEF    "main"
#define DECL_LANG_DEF    "decl"
#define VAR_LANG_DEF     "var"
#define RETURN_LANG_DEF  "return"
#define PRINTF_LANG_DEF  "printf"
#define SCANF_LANG_DEF   "scanf"
#define IF_LANG_DEF      "if"
#define ELSE_LANG_DEF    "else"
#define FOR_LANG_DEF     "for"
#define WHILE_LANG_DEF   "while"
#define BREAK_LANG_DEF   "break"
#define SQRT_LANG_DEF    "sqrt"


#endif
