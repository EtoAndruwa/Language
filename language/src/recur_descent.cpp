// #include "language.h"

// Node* get_main(Tree* tree_ptr, token* tok_arr_ptr, FILE* log_ptr)
// {   
//     if(tok_arr_ptr[tree_ptr->cur_tok].token_type)
//     {


//     }



// }

// Node* get_recur_tree(Tree* tree_ptr, token* tok_arr_ptr) // ok
// {
//     FILE* log_ptr = fopen("parser_log.txt","w");
//     if(log_ptr == nullptr)
//     {
//         ERROR_MESSAGE(stderr, ERR_OPEN_PARSER_LOG)
//         return nullptr;
//     }

//     Node* root_node = get_main(tree_ptr, tok_arr_ptr, log_ptr);

//     if(fclose(log_ptr) == EOF)
//     {
//         ERROR_MESSAGE(stderr, ERR_CLOSE_PARSER_LOG)
//         return nullptr;
//     }

//     return root_node;
// }

