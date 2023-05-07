#ifndef DSL_LEXER_H
#define DSL_LEXER_H

#define LEX_ERROR       lexer_str_ptr->error_code
#define LEX_TOKS        lexer_str_ptr->tok_arr_ptr
#define STRING          lexer_str_ptr->buff_ptr
#define POSITION        lexer_str_ptr->cur_pos_buff
#define CUR_TOK         lexer_str_ptr->cur_tok

#endif
