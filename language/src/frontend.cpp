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

    Node* expres = get_express(tree_ptr, tok_arr_ptr);

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
    return MAIN_NODE(expres);
}

// int get_line_end(Tree* tree_ptr, token* tok_arr_ptr)
// {
//     if()
//     {


//     }   
// }

Node* get_express(Tree* tree_ptr, token* tok_arr_ptr)
{
    return get_decl_var(tree_ptr, tok_arr_ptr);
}

// Node* get_op(Tree* tree_ptr, token* tok_arr_ptr)
// {


    
// }

// Node* get_var(Tree* tree_ptr, token* tok_arr_ptr)
// {


    
// }

// Node* get_val(Tree* tree_ptr, token* tok_arr_ptr)
// {


    
// }

// Node* get_assign(Tree* tree_ptr, token* tok_arr_ptr)
// {
//     if()
//     {


//     }

Node* get_decl_var(Tree* tree_ptr, token* tok_arr_ptr)
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

        Node* val = VAL_NODE(tok_arr_ptr[TREE_CUR_TOK].token_value.flt_val);

        TREE_CUR_TOK++;

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
        equal->left_child  = decl_var;
        equal->right_child = val;
        return equal;
    }
    return nullptr;
}

    



Node* get_recur_tree(Tree* tree_ptr, token* tok_arr_ptr) // ok
{
    Node* root_node = get_main(tree_ptr, tok_arr_ptr);

    return root_node;
}

