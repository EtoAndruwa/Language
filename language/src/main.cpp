#include "language.h"
#include "lexer.h"


int main()
{
    lexer_struct lexer_str_ptr = {};
    ctor_lexer(&lexer_str_ptr);

    get_string(&lexer_str_ptr, "parse_this.txt");


    dtor_lexer(&lexer_str_ptr);

    return 0;
}