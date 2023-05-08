#include "language.h"

Node* get_main(Tree* tree_ptr, token* tok_arr_ptr)
{   
    if(tok_arr_ptr[TREE_CUR_TOK].token_type == End_line)
    {
        return EMPTY_NODE();
    }

    if(tok_arr_ptr[TREE_CUR_TOK].token_type != Main)
    {
        ERROR_MESSAGE(stderr, ERR_FRT_NO_MAIN)
        TREE_ERR = ERR_FRT_NO_MAIN;
        return ERROR_NODE();
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
    return MAIN_NODE(express);
}

Node* get_express(Tree* tree_ptr, token* tok_arr_ptr) // CHECKED
{
    Node* inner_of_expr1 = get_decl_var(tree_ptr, tok_arr_ptr);
    if(inner_of_expr1 == nullptr)
    {
        inner_of_expr1 = get_assign(tree_ptr, tok_arr_ptr);

        if(inner_of_expr1->type == ERROR)
        {   
            tree_ptr->error_code = ERR_FRT_INV_VAR_DECL;
            ERROR_MESSAGE(stderr, ERR_FRT_INV_VAR_DECL)
            return inner_of_expr1;
        }
    }
    else if(inner_of_expr1->type == ERROR)
    {   
        tree_ptr->error_code = ERR_FRT_INV_VAR_DECL;
        ERROR_MESSAGE(stderr, ERR_FRT_INV_VAR_DECL)
        return inner_of_expr1;
    }

    Node* expr1 = EXPR_NODE(inner_of_expr1, nullptr);
    Node* store_expr1 = expr1;

    Node* expr2 = nullptr;
    Node* inner_of_expr2 = nullptr; 
    
    while(tok_arr_ptr[TREE_CUR_TOK].token_type != Fig_brack_r)
    {
        inner_of_expr2 = get_decl_var(tree_ptr, tok_arr_ptr);
        if(inner_of_expr2 == nullptr)
        {   
            inner_of_expr2 = get_assign(tree_ptr, tok_arr_ptr);

            if(inner_of_expr2->type == ERROR)
            {   
                tree_ptr->error_code = ERR_FRT_INV_VAR_DECL;
                ERROR_MESSAGE(stderr, ERR_FRT_INV_VAR_DECL)
                return inner_of_expr2;
            }
        }
        else if(inner_of_expr2->type == ERROR)
        {
            tree_ptr->error_code = ERR_FRT_INV_VAR_DECL;
            ERROR_MESSAGE(stderr, ERR_FRT_INV_VAR_DECL)
            return inner_of_expr2;
        }

        expr2 = EXPR_NODE(inner_of_expr2, nullptr);
        expr1->right_child = expr2;
        expr1 = expr1->right_child;
    }

    return store_expr1;
}

Node* rule_E(Tree* tree_ptr, token* tok_arr_ptr) // ok
{
    Node* right_child = nullptr;
    Node* comb_node   = nullptr;
    Node* left_child  = rule_T(tree_ptr, tok_arr_ptr);
    printf("Rule_T1");

    if(left_child == nullptr)
    {

    }

    while(tok_arr_ptr[TREE_CUR_TOK].token_type == Sub || tok_arr_ptr[TREE_CUR_TOK].token_type == Add)
    {
        if(tok_arr_ptr[TREE_CUR_TOK].token_type == Sub)
        {
            TREE_CUR_TOK++;
            comb_node = SUB_NODE(left_child, nullptr);
        }
        else
        {
            TREE_CUR_TOK++;
            comb_node = ADD_NODE(left_child, nullptr);
        }
         
        printf("Rule_T2");
        right_child = rule_T(tree_ptr, tok_arr_ptr);
        comb_node->left_child->right_child = right_child;
        left_child = comb_node;
    }
    return left_child; 
}

Node* rule_T(Tree* tree_ptr, token* tok_arr_ptr) // ok
{
    Node*  right_child = nullptr;
    Node*  comb_node   = nullptr;
    Node*  left_child  = rule_Pow(tree_ptr, tok_arr_ptr);

    if(left_child == nullptr)
    {

        return nullptr;
    }

    while(tok_arr_ptr[TREE_CUR_TOK].token_type == Mul || tok_arr_ptr[TREE_CUR_TOK].token_type == Div)
    {
        if(tok_arr_ptr[TREE_CUR_TOK].token_type == Mul)
        {
            TREE_CUR_TOK++;
            comb_node = MUL_NODE(left_child, nullptr);
        }
        else
        {
            TREE_CUR_TOK++;
            comb_node = DIV_NODE(left_child, nullptr);
        }
         
        right_child = rule_Pow(tree_ptr, tok_arr_ptr);
        comb_node->left_child->right_child = right_child;
        left_child = comb_node;
    }
    return left_child; 
}

Node* rule_P(Tree* tree_ptr, token* tok_arr_ptr) // 
{
    Node* inner_node = nullptr;
    if(tok_arr_ptr[TREE_CUR_TOK].token_type == Brack_l)
    {
        TREE_CUR_TOK++;
        inner_node = rule_E(tree_ptr, tok_arr_ptr);
         
        if(tok_arr_ptr[TREE_CUR_TOK].token_type != Brack_r)
        {

            return nullptr;
        }
        TREE_CUR_TOK++;
    }
    else
    {
        printf("\nelse rule P\n");
        inner_node = rule_N(tree_ptr, tok_arr_ptr);
        if(inner_node == nullptr)
        {
            
        }
    }
    return inner_node;
}

Node* rule_N(Tree* tree_ptr, token* tok_arr_ptr) // 
{
    if(tok_arr_ptr[TREE_CUR_TOK].token_type == Val)
    {   
        printf("Rule_N if");
        Node* val = VAL_NODE(tok_arr_ptr[TREE_CUR_TOK].token_value.flt_val)
        TREE_CUR_TOK++;
        return val;
    }
    return ERROR_NODE();
}

// Node* rule_V(Tree* tree_ptr, token* tok_arr_ptr) // 
// {

//     if(tok_arr_ptr(POSITION) != '(' && old_pos != POSITION) // closing bracket is on pos old_pos if func
//     {
//         log_ptr, RULE_V, RULE_V_WAIT, RULE_OK);

//         for(size_t cur_var = 0; cur_var < tree_ptr->num_of_vars; cur_var++)
//         {
//             if(strcmp(var_name, tree_ptr->vars_arr[cur_var].var_text) == 0)
//             {
//                 return create_node(tree_ptr, 0, IS_VARIB, var_name);
//             }
//         }

//         return create_node(tree_ptr, 0, IS_CNST_VAR, var_name);
//     }
//     else
//     {
//         POSITION = old_pos;
//         log_ptr, RULE_V, RULE_V_WAIT, RULE_OK)
//         return rule_F(tree_ptr, tok_arr_ptr);
//     }
// }

// Node* rule_F(Tree* tree_ptr, token* tok_arr_ptr) // 
// {
//     char var_name[MAX_LEN_VARIB];
//     size_t var_name_pos = 0;
//     size_t svd_pos = POSITION;
//     Node* inner_func = nullptr;
//     Node* func_node = nullptr;

//     while(isalpha(tok_arr_ptr(POSITION)) != 0)
//     {
//         var_name[var_name_pos] = tok_arr_ptr(POSITION);
//         var_name_pos++;
//         POSITION++;
//     }
//     var_name[var_name_pos] = '\0';
     

//     if(tok_arr_ptr(POSITION) == '(' && POSITION != svd_pos)
//     {    
//         log_ptr, RULE_F, RULE_F_WAIT, RULE_OK)
//         int ex_func = NON_EXIST_FUNC;
        
//         POSITION += 1;
         
//         inner_func = rule_E(tree_ptr, tok_arr_ptr);
         

//         func_node  = nullptr;

//         #define DEF_FUNC(name, code, str_val)                                           \
//             if(strcasecmp(var_name, str_val) == 0)                                      \
//             {                                                                           \
//                 func_node = create_node(tree_ptr, name, IS_FUNC, "", inner_func);       \
//                 ex_func = EXISTING_FUNC;                                                \
//             }                                                                           \

//         #define DEF_OP(name, code, str_val)

//         #include "def_cmd.h"
//         #undef DEF_FUNC
//         #undef DEF_OP

//         if(ex_func == NON_EXIST_FUNC)
//         {
//             log_ptr, RULE_F, RULE_F_WAIT, RULE_F_ERR)
//             ERROR_MESSAGE(stderr, NON_EXIST_FUNC);
//             return nullptr;
//         }

//         if(tok_arr_ptr(POSITION) != ')')
//         {
//             log_ptr, RULE_F, RULE_F_WAIT, RULE_F_ERR)
//             ERROR_MESSAGE(stderr, ERR_NO_CLOSING_BRACKETS)
//             return nullptr;
//         } 
//         POSITION++;
         
//         return func_node;
//     }
// }

Node* rule_Pow(Tree* tree_ptr, token* tok_arr_ptr) // 
{
    Node* pow = rule_P(tree_ptr, tok_arr_ptr);

    while(tok_arr_ptr[TREE_CUR_TOK].token_type == Pow)
    {
        TREE_CUR_TOK++;
        Node* exp = rule_P(tree_ptr, tok_arr_ptr); 
        pow = POW_NODE(pow, exp);
    }
    return pow;
}

Node* get_assign(Tree* tree_ptr, token* tok_arr_ptr) // MUST BE CHANGED
{
    if(tok_arr_ptr[TREE_CUR_TOK].token_type == Word && tok_arr_ptr[TREE_CUR_TOK + 1].token_type != Brack_l)
    {
        Node* var = VAR_NODE(tok_arr_ptr[TREE_CUR_TOK].token_text);
        TREE_CUR_TOK++;
        if(tok_arr_ptr[TREE_CUR_TOK].token_type != Eq)
        {
            tree_ptr->error_code = ERR_FRT_INV_ASSIGNMENT;
            ERROR_MESSAGE(stderr, ERR_FRT_INV_ASSIGNMENT)
            return ERROR_NODE()
        }

        Node* equal = EQ_NODE(nullptr, nullptr)

        TREE_CUR_TOK++;
        if(tok_arr_ptr[TREE_CUR_TOK].token_type != Brack_l)
        {
            tree_ptr->error_code = ERR_FRT_INV_VAR_DECL;
            ERROR_MESSAGE(stderr, ERR_FRT_INV_VAR_DECL)
            return ERROR_NODE()
        }
        TREE_CUR_TOK++;

        Node* node_E = rule_E(tree_ptr, tok_arr_ptr); 

        if(tok_arr_ptr[TREE_CUR_TOK].token_type != Brack_r)
        {
            tree_ptr->error_code = ERR_FRT_INV_VAR_DECL;
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

Node* get_decl_var(Tree* tree_ptr, token* tok_arr_ptr) // MUST BE CHANGED
{
    if(tok_arr_ptr[TREE_CUR_TOK].token_type == Var)
    {
        TREE_CUR_TOK++;
        if(tok_arr_ptr[TREE_CUR_TOK].token_type != Word)
        {
            tree_ptr->error_code = ERR_FRT_INV_VAR_DECL;
            ERROR_MESSAGE(stderr, ERR_FRT_INV_VAR_DECL)
            return ERROR_NODE()
        }
        Node* decl_var = DECL_VAR_NODE(tok_arr_ptr[TREE_CUR_TOK].token_text);

        TREE_CUR_TOK++;
        if(tok_arr_ptr[TREE_CUR_TOK].token_type != Eq)
        {
            tree_ptr->error_code = ERR_FRT_INV_VAR_DECL;
            ERROR_MESSAGE(stderr, ERR_FRT_INV_VAR_DECL)
            return ERROR_NODE()
        }
        Node* equal = EQ_NODE(nullptr, nullptr)

        TREE_CUR_TOK++;
        if(tok_arr_ptr[TREE_CUR_TOK].token_type != Brack_l)
        {
            tree_ptr->error_code = ERR_FRT_INV_VAR_DECL;
            ERROR_MESSAGE(stderr, ERR_FRT_INV_VAR_DECL)
            return ERROR_NODE()
        }
        TREE_CUR_TOK++;

        Node* node_E = rule_E(tree_ptr, tok_arr_ptr); 

        if(tok_arr_ptr[TREE_CUR_TOK].token_type != Brack_r)
        {
            tree_ptr->error_code = ERR_FRT_INV_VAR_DECL;
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

Node* get_recur_tree(Tree* tree_ptr, token* tok_arr_ptr) // ok
{
    Node* root_node = get_main(tree_ptr, tok_arr_ptr);

    return root_node;
}

