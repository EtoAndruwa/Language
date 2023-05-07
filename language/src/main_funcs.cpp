#include "language.h"

// int output_tree(const Node* const root_node_ptr, char* file_name) // ok
// {
//     FILE* file_ptr = fopen(file_name, "w");
//     if(file_ptr == nullptr)
//     {
//         ERROR_MESSAGE(stderr, ERR_CANNOT_OPEN_OUTPUT)
//         return ERR_CANNOT_OPEN_OUTPUT;
//     }

//     print_recur_tree(root_node_ptr, file_ptr);

//     if(fclose(file_ptr) == EOF)
//     {
//         ERROR_MESSAGE(stderr, ERR_CANNOT_CLOSE_OUTPUT)
//         return ERR_CANNOT_CLOSE_OUTPUT;
//     }
// }

// Node* input_tree_old(Tree* tree_ptr) // OLD
// {
//     if(tree_ptr->cur_tok < tree_ptr->num_of_toks)
//     {
//         if(tree_ptr->toks[tree_ptr->cur_tok].type == IS_VAL)
//         {   
//             GET_CUR_TOK()
//             return NUM_NODE(tree_ptr->toks[cur_tok].value.flt_val)
//         }
//         if(tree_ptr->toks[tree_ptr->cur_tok].type == IS_VARIB)
//         {
//             GET_CUR_TOK()
//             return VARIB_NODE(tree_ptr->toks[cur_tok].text)
//         }
//         if(tree_ptr->toks[tree_ptr->cur_tok].type == IS_CNST_VAR)
//         {
//             GET_CUR_TOK()
//             return CNST_VARIB_NODE(tree_ptr->toks[cur_tok].text)
//         }

//         Node* left  = nullptr;
//         Node* right = nullptr;
//         switch(tree_ptr->toks[tree_ptr->cur_tok].value.int_val)
//         {
//             #define DEF_OP(code, ...)                                               \
//                 case code: tree_ptr->cur_tok++;                                     \
//                     left  = input_tree_old(tree_ptr);                               \
//                     right = input_tree_old(tree_ptr);                               \
//                     return create_node(tree_ptr, code, IS_OP, "", left, right);     \

//             /*
//                 For POW func we need to read two arguments
//                 base and exp therefore is statement included 
//                 into the macro
//             */
//             #define DEF_FUNC(code, ...)                                                     \
//                 case code: tree_ptr->cur_tok++;                                             \
//                     if(code == Pow)                                                         \
//                     {                                                                       \
//                         left  = input_tree_old(tree_ptr);                                   \
//                         right = input_tree_old(tree_ptr);                                   \
//                         return create_node(tree_ptr, code, IS_FUNC, "", left, right);       \
//                     }                                                                       \
//                     left = input_tree_old(tree_ptr);                                        \
//                     return create_node(tree_ptr, code, IS_FUNC, "", left);                  \

//             #include "def_cmd.h"
//             #undef DEF_OP
//             #undef DEF_FUNC

//             default:
//                 ERROR_MESSAGE(stderr, ERR_UNKNOWN_FUNC)
//                 tree_ptr->error_code = ERR_UNKNOWN_FUNC;
//                 return nullptr;
//         }
//     }
// }

size_t check_is_float(char* num_text) // ok
{
    size_t is_digits = IS_FLOAT; 
    size_t length_text = strlen(num_text);

    if(num_text[0] == '-' && length_text == 1) // case for '-' minus sign only
    {
        return NOT_ALL_DIGITS;
    }
    if(num_text[0] != '-' && isdigit(num_text[0]) == 0) // case for negative float
    {    
        return NOT_ALL_DIGITS;
    }
    for(size_t i  = 1; i < length_text; i++)
    {
        if(isdigit(num_text[i]) == 0 && num_text[i] != '.') // If the character is not a digit
        {
            is_digits = NOT_ALL_DIGITS;
        }
    }

    return is_digits; // Returns IS_FLOAT if the word if float
}

