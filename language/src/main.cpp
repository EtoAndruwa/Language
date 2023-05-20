#include "language.h"

int main()
{
    Lexer_struct lexer_str_ptr = {};
    Tree_struct tree_str_ptr = {};
    Backend_struct backend_str_ptr = {};

    ctor_tree(&tree_str_ptr);
    ctor_lexer(&lexer_str_ptr);
    ctor_backend(&backend_str_ptr);

    get_string(&lexer_str_ptr, "parse_this.txt");
    get_toks(&lexer_str_ptr);
    // print_toks(&lexer_str_ptr);

    tree_str_ptr.root = get_recur_tree(&tree_str_ptr, &lexer_str_ptr,lexer_str_ptr.tok_arr_ptr);
    create_graph_jpg(&tree_str_ptr, "tree_str_ptr");
    create_html(OUTPUT_NAME);

    find_main_node(&backend_str_ptr, tree_str_ptr.root); // puts all decls and main into right order
    create_asm(&backend_str_ptr);

    print_decls(&backend_str_ptr);
    print_funcs(&backend_str_ptr);
    print_vars(&backend_str_ptr);


    dtor_backend(&backend_str_ptr);
    dtor_lexer(&lexer_str_ptr);
    dtor_tree(&tree_str_ptr);

    return 0;
}