#include "lexer.h"

int ctor_lexer(lexer_struct* lexer_str_ptr) // CHECKED
{
    if(lexer_str_ptr == nullptr)
    {
        ERROR_MESSAGE(stderr, ERR_LEX_CALLOC_LEXER)
        return ERR_LEX_CALLOC_LEXER;
    }

    lexer_str_ptr->cur_pos_buff = 0;
    lexer_str_ptr->error_code   = LEXER_OK;   
    lexer_str_ptr->buff_ptr     = nullptr;
    lexer_str_ptr->tok_arr_ptr  = nullptr;
    lexer_str_ptr->buff_size    = 0;
    lexer_str_ptr->num_of_toks  = 10;

    lexer_str_ptr->tok_arr_ptr = (token*)calloc(lexer_str_ptr->num_of_toks, sizeof(token));
    if(lexer_str_ptr == nullptr)
    {
        lexer_str_ptr->error_code = ERR_LEX_CALLOC_TOK_ARR;
        ERROR_MESSAGE(stderr, lexer_str_ptr->error_code)
        return lexer_str_ptr->error_code;
    }

    return LEX_RET_OK;
}

int dtor_lexer(lexer_struct* lexer_str_ptr) // CHECKED
{   
    if(lexer_str_ptr->tok_arr_ptr != nullptr)
    {
        for(size_t i = 0; i < lexer_str_ptr->num_of_toks; i++)
        {
            stpcpy(lexer_str_ptr->tok_arr_ptr[i].token_text, "");
            lexer_str_ptr->tok_arr_ptr[i].token_type = LEX_POISON;
            lexer_str_ptr->tok_arr_ptr[i].token_value.float_val = LEX_POISON;
        }

        free(lexer_str_ptr->tok_arr_ptr);
        lexer_str_ptr->tok_arr_ptr = nullptr;        
    }

    lexer_str_ptr->cur_pos_buff = LEX_POISON;
    lexer_str_ptr->error_code   = LEX_POISON;   

    free(lexer_str_ptr->buff_ptr);
    lexer_str_ptr->buff_ptr  = nullptr;

    lexer_str_ptr->buff_size    = LEX_POISON;
    lexer_str_ptr->num_of_toks  = LEX_POISON;

    return LEX_RET_OK;
}

int get_string(lexer_struct* lexer_str_ptr, char* file_name) // CHECKED
{
    FILE* file_inp_ptr = fopen(file_name, "rb");
    if(file_inp_ptr == nullptr)
    {
        lexer_str_ptr->error_code = ERR_LEX_OPEN_INP_FILE;
        ERROR_MESSAGE(stderr, lexer_str_ptr->error_code)
        return lexer_str_ptr->error_code;
    }

    int error_code = get_size(lexer_str_ptr, file_inp_ptr);
    if(error_code != LEX_RET_OK)
    {
        ERROR_MESSAGE(stderr, error_code)
        return error_code;
    }

    error_code = get_into_buff(lexer_str_ptr, file_inp_ptr);
    if(error_code != LEX_RET_OK)
    {
        ERROR_MESSAGE(stderr, error_code)
        return error_code;
    }

    printf("Buffer: %s\n", lexer_str_ptr->buff_ptr);

    if(fclose(file_inp_ptr) == EOF)
    {   
        lexer_str_ptr->error_code = ERR_LEX_CLOSE_INP_FILE;
        ERROR_MESSAGE(stderr, lexer_str_ptr->error_code)
        return lexer_str_ptr->error_code;
    }
    return LEX_RET_OK;
}

int get_size(lexer_struct* lexer_str_ptr, FILE* file_inp_ptr) // CHECKED
{
    fseek(file_inp_ptr, 0, SEEK_END); // Puts the pointer inside the file to the end of it
    lexer_str_ptr->buff_size = ftell(file_inp_ptr); // Get the size of the file with '\r'!

    if(lexer_str_ptr->buff_size == 0) 
    {
        lexer_str_ptr->error_code = ERR_LEX_EMPTY_FILE; 
        ERROR_MESSAGE(stderr, lexer_str_ptr->error_code)
        return lexer_str_ptr->error_code;
    } 

    fseek(file_inp_ptr, 0, SEEK_SET); // Puts the pointer inside the file to the start

    return LEX_RET_OK;
}

int get_into_buff(lexer_struct* lexer_str_ptr, FILE* file_inp_ptr) // CHECKED
{
    fseek(file_inp_ptr, 0, SEEK_SET); // Puts the pointer inside the file to the start
    lexer_str_ptr->buff_ptr = (char*)calloc(1, sizeof(char) * (lexer_str_ptr->buff_size + 1)); // Allocates enough memmory for the buffer of chars  

    if(lexer_str_ptr->buff_ptr == nullptr)
    {
        lexer_str_ptr->error_code = ERR_LEX_CALLOC_BUFF;
        ERROR_MESSAGE(stderr, lexer_str_ptr->error_code)
        return lexer_str_ptr->error_code;
    }

    int num_read = fread(lexer_str_ptr->buff_ptr, sizeof(char), lexer_str_ptr->buff_size, file_inp_ptr); // Reads the file into the buffer
    
    if((num_read <= 0) && (num_read > lexer_str_ptr->buff_size))
    {
        lexer_str_ptr->error_code = ERR_LEX_READ_INTO_BUFF;
        ERROR_MESSAGE(stderr, lexer_str_ptr->error_code)
        return lexer_str_ptr->error_code;       
    }
    
    lexer_str_ptr->buff_ptr[lexer_str_ptr->buff_size] = '\0'; // Makes form the file null-terminated string
    fseek(file_inp_ptr, 0, SEEK_SET); // Puts the pointer inside the file to the start
    
    return LEX_RET_OK;
}

// size_t skip_spaces(Tree* lexer_str_ptr) // ok
// {
//     while(isspace(STRING(POSITION)) != 0)
//     {
//         POSITION++;
//     }

//     return POSITION;
// }
