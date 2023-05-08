#include "language.h"

Node* get_main(Tree* tree_ptr, token* tok_arr_ptr)
{   
    if(tok_arr_ptr[TREE_CUR_TOK].token_type != Main)
    {
        ERROR_MESSAGE(stderr, ERR_FRT_NO_MAIN)
        TREE_ERR = ERR_FRT_NO_MAIN;
        return nullptr;
    }
    TREE_CUR_TOK++;
    if(tok_arr_ptr[TREE_CUR_TOK].token_type != Brack_l && tok_arr_ptr[TREE_CUR_TOK + 1].token_type != Brack_r)
    {
        ERROR_MESSAGE(stderr, ERR_FRT_MAIN_BRACK)
        TREE_ERR = ERR_FRT_MAIN_BRACK;
        return nullptr;
    }
    TREE_CUR_TOK++;
    if(tok_arr_ptr[TREE_CUR_TOK].token_type != Sqr_brack_l)
    {
        ERROR_MESSAGE(stderr, ERR_FRT_NO_OPEN_SQR_BR)
        TREE_ERR = ERR_FRT_NO_OPEN_SQR_BR;
        return nullptr;
    }
    TREE_CUR_TOK++;

    Node* expres = get_express(tree_ptr, tok_arr_ptr);

    if(tok_arr_ptr[TREE_CUR_TOK].token_type != Sqr_brack_r)
    {
        ERROR_MESSAGE(stderr, ERR_FRT_NO_CLOS_SQR_BR)
        TREE_ERR = ERR_FRT_NO_CLOS_SQR_BR;
        return nullptr;
    }
    TREE_CUR_TOK++;

    create_node(tree_ptr, 0, IS_MAIN, nullptr, );
}

int get_line_end(Tree* tree_ptr, token* tok_arr_ptr)
{
    if()
    {


    }   
}

Node* get_express()
{


    
}


Node* get_recur_tree(Tree* tree_ptr, token* tok_arr_ptr) // ok
{
    Node* root_node = get_main(tree_ptr, tok_arr_ptr);

    return root_node;
}

