#include "language.h"

int num_main = 0; // global value for counting main funcs

Node* get_main(Tree* tree_ptr, token* tok_arr_ptr)
{   
    if(tok_arr_ptr[TREE_CUR_TOK].token_type == End_line)
    {
        return EMPTY_NODE();
    }
    
    if(tok_arr_ptr[TREE_CUR_TOK].token_type != Main && tok_arr_ptr[TREE_CUR_TOK].token_type == Decl)
    {
        return nullptr;
    }

    if(num_main >= 1)
    {
        TREE_ERR = ERR_FRT_MULTIPLE_DEF_MAIN;
        ERROR_MESSAGE(stderr, ERR_FRT_MULTIPLE_DEF_MAIN)
        return ERROR_NODE()
    }
    TREE_CUR_TOK++;
    if(tok_arr_ptr[TREE_CUR_TOK].token_type != Brack_l && tok_arr_ptr[TREE_CUR_TOK + 1].token_type != Brack_r)
    {
        ERROR_MESSAGE(stderr, ERR_FRT_MAIN_BRACK)
        TREE_ERR = ERR_FRT_MAIN_BRACK;
        return ERROR_NODE();
    }
    TREE_CUR_TOK += 2;

    if(tok_arr_ptr[TREE_CUR_TOK].token_type != Fig_brack_l)
    {
        ERROR_MESSAGE(stderr, ERR_FRT_NO_OPEN_FIG_BR)
        TREE_ERR = ERR_FRT_NO_OPEN_FIG_BR;
        return ERROR_NODE();
    }
    TREE_CUR_TOK++;

    Node* express = get_express(tree_ptr, tok_arr_ptr);

    if(tok_arr_ptr[TREE_CUR_TOK].token_type != Fig_brack_r)
    {
        ERROR_MESSAGE(stderr, ERR_FRT_NO_CLOS_FIG_BR)
        TREE_ERR = ERR_FRT_NO_CLOS_FIG_BR;
        return ERROR_NODE();
    }
    TREE_CUR_TOK++;

    if(tok_arr_ptr[TREE_CUR_TOK].token_type != End_line)
    {
        ERROR_MESSAGE(stderr, ERR_FRT_NO_END_LINE)
        TREE_ERR = ERR_FRT_NO_END_LINE;
        return ERROR_NODE();
    }
    num_main++;
    TREE_CUR_TOK++;
    return MAIN_NODE(express);
}

Node* get_express(Tree* tree_ptr, token* tok_arr_ptr) // CHECKED
{
    int num_rets = 0;

    Node* inner_of_expr1 = get_decl_var(tree_ptr, tok_arr_ptr);
    if(inner_of_expr1 == nullptr)
    {
        inner_of_expr1 = get_assign(tree_ptr, tok_arr_ptr);

        if(inner_of_expr1 == nullptr)
        {   
            inner_of_expr1 = get_return(tree_ptr, tok_arr_ptr);
            if(inner_of_expr1->type == ERROR)
            {   
                TREE_ERR = ERR_FRT_INV_VAR_DECL;
                ERROR_MESSAGE(stderr, ERR_FRT_INV_VAR_DECL)
                return inner_of_expr1;
            }
            num_rets++;
        }
        else if(inner_of_expr1->type == ERROR)
        {   
            TREE_ERR = ERR_FRT_INV_VAR_DECL;
            ERROR_MESSAGE(stderr, ERR_FRT_INV_VAR_DECL)
            return inner_of_expr1;
        }
    }
    else if(inner_of_expr1->type == ERROR)
    {   
        TREE_ERR = ERR_FRT_INV_VAR_DECL;
        ERROR_MESSAGE(stderr, ERR_FRT_INV_VAR_DECL)
        return inner_of_expr1;
    }

    Node* expr1 = EXPR_NODE(inner_of_expr1, nullptr)
    Node* store_expr1 = expr1;

    Node* expr2 = nullptr;
    Node* inner_of_expr2 = nullptr; 
    
    while(tok_arr_ptr[TREE_CUR_TOK].token_type != Fig_brack_r)
    {
        inner_of_expr2 = get_decl_var(tree_ptr, tok_arr_ptr);
        if(inner_of_expr2 == nullptr)
        {   
            inner_of_expr2 = get_assign(tree_ptr, tok_arr_ptr);

            if(inner_of_expr2 == nullptr)
            {   
                inner_of_expr2 = get_return(tree_ptr, tok_arr_ptr);
                if(inner_of_expr2->type == ERROR)
                {   
                    TREE_ERR = ERR_FRT_INV_VAR_DECL;
                    ERROR_MESSAGE(stderr, ERR_FRT_INV_VAR_DECL)
                    return inner_of_expr2;
                }
                num_rets++;
            }
            else if(inner_of_expr2->type == ERROR)
            {   
                TREE_ERR = ERR_FRT_INV_VAR_DECL;
                ERROR_MESSAGE(stderr, ERR_FRT_INV_VAR_DECL)
                return inner_of_expr2;
            }
        }
        else if(inner_of_expr2->type == ERROR)
        {
            TREE_ERR = ERR_FRT_INV_VAR_DECL;
            ERROR_MESSAGE(stderr, ERR_FRT_INV_VAR_DECL)
            return inner_of_expr2;
        }

        expr2 = EXPR_NODE(inner_of_expr2, nullptr)
        expr1->right_child = expr2;
        expr1 = expr1->right_child;
    }

    if(num_rets == 0)
    {
        TREE_ERR = ERR_FRT_NO_RETURN_FUNC;
        ERROR_MESSAGE(stderr, ERR_FRT_NO_RETURN_FUNC)
        return ERROR_NODE()
    }

    return store_expr1;
}

Node* rule_E(Tree* tree_ptr, token* tok_arr_ptr) // CHECKED
{
    Node* right_child = nullptr;
    Node* comb_node   = nullptr;
    Node* left_child  = rule_T(tree_ptr, tok_arr_ptr);

    if(left_child->type == ERROR)
    {
        TREE_ERR = ERR_FRT_RECUR_RULE_T;
        ERROR_MESSAGE(stderr, ERR_FRT_RECUR_RULE_T)
        return left_child;
    }

    while(tok_arr_ptr[TREE_CUR_TOK].token_type == Sub || tok_arr_ptr[TREE_CUR_TOK].token_type == Add)
    {
        if(tok_arr_ptr[TREE_CUR_TOK].token_type == Sub)
        {
            TREE_CUR_TOK++;
            comb_node = SUB_NODE(left_child, nullptr)
        }
        else
        {
            TREE_CUR_TOK++;
            comb_node = ADD_NODE(left_child, nullptr)
        }
         
        right_child = rule_T(tree_ptr, tok_arr_ptr);
        if(left_child->type == ERROR)
        {
            TREE_ERR = ERR_FRT_RECUR_RULE_T;
            ERROR_MESSAGE(stderr, ERR_FRT_RECUR_RULE_T)
            return left_child;
        }

        comb_node->left_child->right_child = right_child;
        left_child = comb_node;
    }
    return left_child; 
}

Node* rule_T(Tree* tree_ptr, token* tok_arr_ptr) // CHECKED
{
    Node*  right_child = nullptr;
    Node*  comb_node   = nullptr;
    Node*  left_child  = rule_Pow(tree_ptr, tok_arr_ptr);

    if(left_child->type == ERROR)
    {
        TREE_ERR = ERR_FRT_RECUR_RULE_POW;
        ERROR_MESSAGE(stderr, ERR_FRT_RECUR_RULE_POW)
        return left_child;
    }

    while(tok_arr_ptr[TREE_CUR_TOK].token_type == Mul || tok_arr_ptr[TREE_CUR_TOK].token_type == Div)
    {
        if(tok_arr_ptr[TREE_CUR_TOK].token_type == Mul)
        {
            TREE_CUR_TOK++;
            comb_node = MUL_NODE(left_child, nullptr)
        }
        else
        {
            TREE_CUR_TOK++;
            comb_node = DIV_NODE(left_child, nullptr)
        }
         
        right_child = rule_Pow(tree_ptr, tok_arr_ptr);
        if(right_child->type == ERROR)
        {
            TREE_ERR = ERR_FRT_RECUR_RULE_POW;
            ERROR_MESSAGE(stderr, ERR_FRT_RECUR_RULE_POW)
            return right_child;
        }

        comb_node->left_child->right_child = right_child;
        left_child = comb_node;
    }
    return left_child; 
}

Node* rule_P(Tree* tree_ptr, token* tok_arr_ptr) // CHECKED
{
    Node* inner_node = nullptr;
    if(tok_arr_ptr[TREE_CUR_TOK].token_type == Brack_l)
    {
        TREE_CUR_TOK++;
        inner_node = rule_E(tree_ptr, tok_arr_ptr);
        if(inner_node->type == ERROR)
        {
            TREE_ERR = ERR_FRT_RECUR_RULE_E;
            ERROR_MESSAGE(stderr, ERR_FRT_RECUR_RULE_E)
            return inner_node;
        }
         
        if(tok_arr_ptr[TREE_CUR_TOK].token_type != Brack_r)
        {
            ERROR_MESSAGE(stderr, ERR_FRT_NO_CLOS_BR)
            TREE_ERR = ERR_FRT_NO_CLOS_BR;
            return ERROR_NODE();
        }
        TREE_CUR_TOK++;
    }
    else
    {
        inner_node = rule_N(tree_ptr, tok_arr_ptr);
        if(inner_node == nullptr)
        {
            inner_node = rule_V(tree_ptr, tok_arr_ptr);
            if(inner_node == nullptr)
            {
                inner_node = rule_F(tree_ptr, tok_arr_ptr);
            }
        }
    }
    return inner_node;
}

Node* rule_N(Tree* tree_ptr, token* tok_arr_ptr) // CHECKED
{
    if(tok_arr_ptr[TREE_CUR_TOK].token_type == Val)
    {   
        Node* val = VAL_NODE(tok_arr_ptr[TREE_CUR_TOK].token_value.flt_val)
        TREE_CUR_TOK++;
        return val;
    }
    return nullptr;
}

Node* rule_V(Tree* tree_ptr, token* tok_arr_ptr) // CHECKED
{
    if(tok_arr_ptr[TREE_CUR_TOK].token_type == Word && tok_arr_ptr[TREE_CUR_TOK + 1].token_type != Brack_l) // closing bracket is on pos old_pos if func
    {
        Node* var = VAR_NODE(tok_arr_ptr[TREE_CUR_TOK].token_text)
        TREE_CUR_TOK++;
        return var;
    }   
    return nullptr;
}

Node* rule_F(Tree* tree_ptr, token* tok_arr_ptr) // CHECKED
{
    if(tok_arr_ptr[TREE_CUR_TOK].token_type == Word && tok_arr_ptr[TREE_CUR_TOK + 1].token_type == Brack_l)
    {
        Node* func_name = FUNC_NAME_NODE(tok_arr_ptr[TREE_CUR_TOK].token_text)
        Node* func_info = FUNC_INFO_NODE(nullptr, nullptr)
        TREE_CUR_TOK += 2;
        if(tok_arr_ptr[TREE_CUR_TOK].token_type == Brack_r)
        {
            Node* func_args = FUNC_ARGS_NODE(nullptr, nullptr);
            func_args->left_child = EMPTY_NODE()
            func_info->left_child = func_args;
        }
        else
        {
            Node* func_args = FUNC_ARGS_NODE(nullptr, nullptr);
            Node* first_arg = rule_E(tree_ptr, tok_arr_ptr);

            if(first_arg->type == ERROR)
            {
                TREE_ERR = ERR_FRT_INV_ARGS_FUNC;
                ERROR_MESSAGE(stderr, ERR_FRT_INV_ARGS_FUNC)
                return first_arg;
            }

            Node* save_func_args = func_args;
            func_args->left_child = first_arg;

            while(tok_arr_ptr[TREE_CUR_TOK].token_type != Brack_r)
            {
                if(tok_arr_ptr[TREE_CUR_TOK].token_type == Comma)
                {
                    if(tok_arr_ptr[TREE_CUR_TOK + 1].token_type == Brack_r)
                    {
                        ERROR_MESSAGE(stderr, ERR_FRT_INV_ARGS_FUNC)
                        TREE_ERR = ERR_FRT_INV_ARGS_FUNC;
                        return ERROR_NODE()
                    }
                    TREE_CUR_TOK++;
                    continue;
                }

                Node* func_args2 = FUNC_ARGS_NODE(nullptr, nullptr);
                Node* second_arg = rule_E(tree_ptr, tok_arr_ptr);

                if(second_arg->type == ERROR)
                {
                    TREE_ERR = ERR_FRT_INV_ARGS_FUNC;
                    ERROR_MESSAGE(stderr, ERR_FRT_INV_ARGS_FUNC)
                    return second_arg;
                }

                func_args2->left_child = second_arg;
                func_args->right_child = func_args2;
                func_args = func_args2;
                if(tok_arr_ptr[TREE_CUR_TOK].token_type != Comma && tok_arr_ptr[TREE_CUR_TOK].token_type != Brack_r)
                {
                    ERROR_MESSAGE(stderr, ERR_FRT_NO_CLOS_BR)
                    TREE_ERR = ERR_FRT_NO_CLOS_BR;
                    return ERROR_NODE()
                }
            }
            func_info->left_child = save_func_args;
        }
        func_info->right_child = EMPTY_NODE();
        Node* func_head = FUNC_HEAD_NODE(func_name, func_info)
        TREE_CUR_TOK++;
        return FUNC_CALL_NODE(func_head)
    }
    return ERROR_NODE();
}

Node* rule_Pow(Tree* tree_ptr, token* tok_arr_ptr) // CHECKED
{
    Node* pow = rule_P(tree_ptr, tok_arr_ptr);
    if(pow->type == ERROR)
    {
        TREE_ERR = ERR_FRT_RECUR_RULE_P;
        ERROR_MESSAGE(stderr, ERR_FRT_RECUR_RULE_P)
        return pow;
    }

    while(tok_arr_ptr[TREE_CUR_TOK].token_type == Pow)
    {
        TREE_CUR_TOK++;
        Node* exp = rule_P(tree_ptr, tok_arr_ptr); 

        if(exp->type == ERROR)
        {
            TREE_ERR = ERR_FRT_RECUR_RULE_P;
            ERROR_MESSAGE(stderr, ERR_FRT_RECUR_RULE_P)
            return exp;
        }
        pow = POW_NODE(pow, exp);
    }
    return pow;
}

Node* get_assign(Tree* tree_ptr, token* tok_arr_ptr) // CHECKED
{
    if(tok_arr_ptr[TREE_CUR_TOK].token_type == Word && tok_arr_ptr[TREE_CUR_TOK + 1].token_type != Brack_l)
    {
        Node* var = VAR_NODE(tok_arr_ptr[TREE_CUR_TOK].token_text);
        TREE_CUR_TOK++;
        if(tok_arr_ptr[TREE_CUR_TOK].token_type != Eq)
        {
            TREE_ERR = ERR_FRT_INV_ASSIGNMENT;
            ERROR_MESSAGE(stderr, ERR_FRT_INV_ASSIGNMENT)
            return ERROR_NODE()
        }

        Node* equal = EQ_NODE(nullptr, nullptr)

        TREE_CUR_TOK++;
        if(tok_arr_ptr[TREE_CUR_TOK].token_type != Brack_l)
        {
            TREE_ERR = ERR_FRT_INV_VAR_DECL;
            ERROR_MESSAGE(stderr, ERR_FRT_INV_VAR_DECL)
            return ERROR_NODE()
        }
        TREE_CUR_TOK++;

        Node* node_E = rule_E(tree_ptr, tok_arr_ptr); 

        if(tok_arr_ptr[TREE_CUR_TOK].token_type != Brack_r)
        {
            TREE_ERR = ERR_FRT_INV_VAR_DECL;
            ERROR_MESSAGE(stderr, ERR_FRT_INV_VAR_DECL)
            return ERROR_NODE()
        }
        TREE_CUR_TOK++;

        if(tok_arr_ptr[TREE_CUR_TOK].token_type != End_line)
        {
            ERROR_MESSAGE(stderr, ERR_FRT_NO_END_LINE)
            TREE_ERR = ERR_FRT_NO_END_LINE;
            return ERROR_NODE()
        }
        TREE_CUR_TOK++;
        equal->left_child->left_child  = var;
        equal->left_child->right_child = node_E;
        return equal;
    }
    return nullptr;
}

Node* get_decl_var(Tree* tree_ptr, token* tok_arr_ptr) // CHECKED
{
    if(tok_arr_ptr[TREE_CUR_TOK].token_type == Var)
    {
        TREE_CUR_TOK++;
        if(tok_arr_ptr[TREE_CUR_TOK].token_type != Word)
        {
            TREE_ERR = ERR_FRT_INV_VAR_DECL;
            ERROR_MESSAGE(stderr, ERR_FRT_INV_VAR_DECL)
            return ERROR_NODE()
        }
        Node* decl_var = DECL_VAR_NODE(tok_arr_ptr[TREE_CUR_TOK].token_text);

        TREE_CUR_TOK++;
        if(tok_arr_ptr[TREE_CUR_TOK].token_type != Eq)
        {
            TREE_ERR = ERR_FRT_INV_VAR_DECL;
            ERROR_MESSAGE(stderr, ERR_FRT_INV_VAR_DECL)
            return ERROR_NODE()
        }
        Node* equal = EQ_NODE(nullptr, nullptr)

        TREE_CUR_TOK++;
        if(tok_arr_ptr[TREE_CUR_TOK].token_type != Brack_l)
        {
            TREE_ERR = ERR_FRT_INV_VAR_DECL;
            ERROR_MESSAGE(stderr, ERR_FRT_INV_VAR_DECL)
            return ERROR_NODE()
        }
        TREE_CUR_TOK++;

        Node* node_E = rule_E(tree_ptr, tok_arr_ptr); 

        if(tok_arr_ptr[TREE_CUR_TOK].token_type != Brack_r)
        {
            TREE_ERR = ERR_FRT_INV_VAR_DECL;
            ERROR_MESSAGE(stderr, ERR_FRT_INV_VAR_DECL)
            return ERROR_NODE()
        }
        TREE_CUR_TOK++;

        if(tok_arr_ptr[TREE_CUR_TOK].token_type != End_line)
        {
            ERROR_MESSAGE(stderr, ERR_FRT_NO_END_LINE)
            TREE_ERR = ERR_FRT_NO_END_LINE;
            return ERROR_NODE()
        }
        TREE_CUR_TOK++;
        equal->left_child->left_child  = decl_var;
        equal->left_child->right_child = node_E;
        return equal;
    }
    return nullptr;
}

Node* get_recur_tree(Tree* tree_ptr, Lexer_struct* lexer_str_ptr,token* tok_arr_ptr) // CHECKED
{
    Node* root_inner = get_main(tree_ptr, tok_arr_ptr);
    if(root_inner == nullptr)
    {   
        root_inner = get_func_decl(tree_ptr, tok_arr_ptr);
        if(root_inner->type == ERROR)
        {
            TREE_ERR = ERR_FRT_INV_DECL_FUNC;
            ERROR_MESSAGE(stderr, ERR_FRT_INV_DECL_FUNC)
            return root_inner;
        }
    }
    else if(root_inner->type == ERROR)
    {
        TREE_ERR = ERR_FRT_INV_DECL_FUNC;
        ERROR_MESSAGE(stderr, ERR_FRT_INV_DECL_FUNC)
        return root_inner;
    }

    Node* root_node = EXPR_NODE(root_inner, nullptr);
    Node* svd_root_node = root_node;
    Node* other_expr = nullptr;
    tree_ptr->num_of_toks = lexer_str_ptr->cur_tok;

    while(TREE_CUR_TOK < tree_ptr->num_of_toks - 1)
    {   
        Node* other_expr_inner = get_main(tree_ptr, tok_arr_ptr);
        if(other_expr_inner == nullptr)
        {   
            other_expr_inner = get_func_decl(tree_ptr, tok_arr_ptr);
            if(other_expr_inner->type == ERROR)
            {
                TREE_ERR = ERR_FRT_INV_DECL_FUNC;
                ERROR_MESSAGE(stderr, ERR_FRT_INV_DECL_FUNC)
                return other_expr_inner;
            }
        }
        else if(other_expr_inner->type == ERROR)
        {
            TREE_ERR = ERR_FRT_INV_DECL_FUNC;
            ERROR_MESSAGE(stderr, ERR_FRT_INV_DECL_FUNC)
            return other_expr_inner;
        }

        other_expr = EXPR_NODE(other_expr_inner, nullptr);
        root_node->right_child = other_expr;
        root_node = root_node->right_child;
    }

    if(num_main == 0)
    {
        TREE_ERR = ERR_FRT_NO_MAIN;
        ERROR_MESSAGE(stderr, ERR_FRT_NO_MAIN)
        return ERROR_NODE()
    }

    return svd_root_node;
}

Node* get_func_decl(Tree* tree_ptr, token* tok_arr_ptr) // CHECKED
{
    if(tok_arr_ptr[TREE_CUR_TOK].token_type == Decl)
    {
        TREE_CUR_TOK++;
        if(tok_arr_ptr[TREE_CUR_TOK].token_type == Word && tok_arr_ptr[TREE_CUR_TOK + 1].token_type == Brack_l)
        {
            Node* func_name = FUNC_NAME_NODE(tok_arr_ptr[TREE_CUR_TOK].token_text);
            Node* func_info = FUNC_INFO_NODE(nullptr, nullptr)
            TREE_CUR_TOK += 2;
            if(tok_arr_ptr[TREE_CUR_TOK].token_type == Brack_r)
            {
                Node* func_args = FUNC_ARGS_NODE(nullptr, nullptr);
                func_args->left_child = EMPTY_NODE()
                func_info->left_child = func_args;
                TREE_CUR_TOK++;
            }
            else
            {
                Node* func_args = FUNC_ARGS_NODE(nullptr, nullptr);
                Node* first_arg = rule_V(tree_ptr, tok_arr_ptr);
                Node* save_func_args = func_args;
                func_args->left_child = first_arg;

                while(tok_arr_ptr[TREE_CUR_TOK].token_type != Brack_r)
                {
                    if(tok_arr_ptr[TREE_CUR_TOK].token_type == Comma)
                    {
                        if(tok_arr_ptr[TREE_CUR_TOK + 1].token_type == Brack_r)
                        {
                            ERROR_MESSAGE(stderr, ERR_FRT_INV_ARGS_FUNC)
                            TREE_ERR = ERR_FRT_INV_ARGS_FUNC;
                            return ERROR_NODE()
                        }
                        TREE_CUR_TOK++;
                        continue;
                    }

                    Node* func_args2 = FUNC_ARGS_NODE(nullptr, nullptr);
                    Node* second_arg = rule_V(tree_ptr, tok_arr_ptr);
                    func_args2->left_child = second_arg;
                    func_args->right_child = func_args2;
                    func_args = func_args2;
                    if(tok_arr_ptr[TREE_CUR_TOK].token_type != Comma && tok_arr_ptr[TREE_CUR_TOK].token_type != Brack_r)
                    {
                        ERROR_MESSAGE(stderr, ERR_FRT_NO_CLOS_BR)
                        TREE_ERR = ERR_FRT_NO_CLOS_BR;
                        return ERROR_NODE()
                    }
                }
                func_info->left_child = save_func_args;
                TREE_CUR_TOK++;
                if(tok_arr_ptr[TREE_CUR_TOK].token_type != Fig_brack_l)
                {
                    TREE_ERR = ERR_FRT_NO_OPEN_FIG_BR;
                    ERROR_MESSAGE(stderr, ERR_FRT_NO_OPEN_FIG_BR)
                    return ERROR_NODE()
                }
                TREE_CUR_TOK++;
                func_info->right_child = get_express(tree_ptr, tok_arr_ptr);
                if(tok_arr_ptr[TREE_CUR_TOK].token_type != Fig_brack_r)
                {
                    TREE_ERR = ERR_FRT_NO_CLOS_FIG_BR;
                    ERROR_MESSAGE(stderr, ERR_FRT_NO_CLOS_FIG_BR)
                    return ERROR_NODE();
                }

                TREE_CUR_TOK++;
                if(tok_arr_ptr[TREE_CUR_TOK].token_type != End_line)
                {
                    ERROR_MESSAGE(stderr, ERR_FRT_NO_END_LINE)
                    TREE_ERR = ERR_FRT_NO_END_LINE;
                    return ERROR_NODE();
                }

                Node* func_head = FUNC_HEAD_NODE(func_name, func_info)
                TREE_CUR_TOK++;
                return FUNC_DECL_NODE(func_head)
            }
        }
        else
        {
            TREE_ERR = ERR_FRT_INV_DECL_FUNC;
            ERROR_MESSAGE(stderr, ERR_FRT_INV_DECL_FUNC)
            return ERROR_NODE()
        }
    }
    return nullptr;
}

Node* get_return(Tree* tree_ptr, token* tok_arr_ptr)
{
    if(tok_arr_ptr[TREE_CUR_TOK].token_type == Return)
    {
        TREE_CUR_TOK++;
        if(tok_arr_ptr[TREE_CUR_TOK].token_type != Brack_l)
        {
            ERROR_MESSAGE(stderr, ERR_FRT_NO_OPEN_BR)
            TREE_ERR = ERR_FRT_NO_OPEN_BR;
            return ERROR_NODE();
        }
        TREE_CUR_TOK++;

        Node* return_val = rule_E(tree_ptr, tok_arr_ptr);
        if(return_val->type == ERROR)
        {
            ERROR_MESSAGE(stderr, ERR_FRT_RECUR_RULE_E)
            TREE_ERR = ERR_FRT_RECUR_RULE_E;
            return return_val;
        }

        Node* expr = EXPR_NODE(return_val, nullptr)   

        if(tok_arr_ptr[TREE_CUR_TOK].token_type != Brack_r)
        {
            ERROR_MESSAGE(stderr, ERR_FRT_NO_CLOS_BR)
            TREE_ERR = ERR_FRT_NO_CLOS_BR;
            return ERROR_NODE();
        }

        TREE_CUR_TOK++;
        if(tok_arr_ptr[TREE_CUR_TOK].token_type != End_line)
        {
            ERROR_MESSAGE(stderr, ERR_FRT_NO_END_LINE)
            TREE_ERR = ERR_FRT_NO_END_LINE;
            return ERROR_NODE();
        }
        TREE_CUR_TOK++;
        
        return RETURN_NODE(expr)
    }
    return ERROR_NODE()
}
