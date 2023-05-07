#include "lexer.h"

int ctor_lexer(lexer_struct* lexer_str_ptr) // CHECKED
{
    if(lexer_str_ptr == nullptr)
    {
        ERROR_MESSAGE(stderr, ERR_LEX_CALLOC_LEXER)
        return ERR_LEX_CALLOC_LEXER;
    }

    lexer_str_ptr->cur_pos_buff = 0;
    LEX_ERROR   = LEXER_OK;   
    lexer_str_ptr->buff_ptr     = nullptr;
    LEX_TOKS  = nullptr;
    lexer_str_ptr->buff_size    = 0;
    lexer_str_ptr->num_of_toks  = 10;

    LEX_TOKS = (token*)calloc(lexer_str_ptr->num_of_toks, sizeof(token));
    if(lexer_str_ptr == nullptr)
    {
        LEX_ERROR = ERR_LEX_CALLOC_TOK_ARR;
        ERROR_MESSAGE(stderr, LEX_ERROR)
        return LEX_ERROR;
    }

    for(size_t i = 0; i < lexer_str_ptr->num_of_toks; i++)
    {
        stpcpy(LEX_TOKS[i].token_text, "");
        LEX_TOKS[i].token_type = EMPTY;
        LEX_TOKS[i].token_value.flt_val = 0;
    }

    return LEX_RET_OK;
}

int dtor_lexer(lexer_struct* lexer_str_ptr) // CHECKED
{   
    if(LEX_TOKS != nullptr)
    {
        for(size_t i = 0; i < lexer_str_ptr->num_of_toks; i++)
        {
            stpcpy(LEX_TOKS[i].token_text, "");
            LEX_TOKS[i].token_type = LEX_POISON;
            LEX_TOKS[i].token_value.flt_val = LEX_POISON;
        }

        free(LEX_TOKS);
        LEX_TOKS = nullptr;        
    }

    lexer_str_ptr->cur_pos_buff = LEX_POISON;
    LEX_ERROR = LEX_POISON;   

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
        LEX_ERROR = ERR_LEX_OPEN_INP_FILE;
        ERROR_MESSAGE(stderr, LEX_ERROR)
        return LEX_ERROR;
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
        LEX_ERROR = ERR_LEX_CLOSE_INP_FILE;
        ERROR_MESSAGE(stderr, LEX_ERROR)
        return LEX_ERROR;
    }
    return LEX_RET_OK;
}

int get_size(lexer_struct* lexer_str_ptr, FILE* file_inp_ptr) // CHECKED
{
    fseek(file_inp_ptr, 0, SEEK_END); // Puts the pointer inside the file to the end of it
    lexer_str_ptr->buff_size = ftell(file_inp_ptr); // Get the size of the file with '\r'!

    if(lexer_str_ptr->buff_size == 0) 
    {
        LEX_ERROR = ERR_LEX_EMPTY_FILE; 
        ERROR_MESSAGE(stderr, LEX_ERROR)
        return LEX_ERROR;
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
        LEX_ERROR = ERR_LEX_CALLOC_BUFF;
        ERROR_MESSAGE(stderr, LEX_ERROR)
        return LEX_ERROR;
    }

    int num_read = fread(lexer_str_ptr->buff_ptr, sizeof(char), lexer_str_ptr->buff_size, file_inp_ptr); // Reads the file into the buffer
    
    if((num_read <= 0) && (num_read > lexer_str_ptr->buff_size))
    {
        LEX_ERROR = ERR_LEX_READ_INTO_BUFF;
        ERROR_MESSAGE(stderr, LEX_ERROR)
        return LEX_ERROR;       
    }

    lexer_str_ptr->buff_ptr[lexer_str_ptr->buff_size] = '\0'; // Makes form the file null-terminated string
    fseek(file_inp_ptr, 0, SEEK_SET); // Puts the pointer inside the file to the start
    
    return LEX_RET_OK;
}

int get_toks(lexer_struct* lexer_str_ptr)
{
    while(POSITION < lexer_str_ptr->buff_size)
    {
        printf("%c ", STRING[POSITION++]);
    }
}

// int realloc_toks()
// {




// }

void print_toks(lexer_struct* lexer_str_ptr)
{
    #define GET_STR(code) #code

    for(size_t i = 0; i < lexer_str_ptr->num_of_toks; i++)
    {
        printf("###############TOKEN_%ld#################\n", i);
        printf("Token text: %s\n", LEX_TOKS[i].token_text);
        printf("Token type: %d (%s)\n", LEX_TOKS[i].token_type, GET_STR(LEX_TOKS[i].token_type));
        printf("Token int_val: %d\n", LEX_TOKS[i].token_value.int_val);
        printf("Token flt_val: %d\n", LEX_TOKS[i].token_value.flt_val);
        printf("###############TOKEN_%ld#################\n", i);
    }
}


// size_t skip_spaces(Tree* lexer_str_ptr) // ok
// {
//     while(isspace(STRING(POSITION)) != 0)
//     {
//         POSITION++;
//     }

//     return POSITION;
// }
