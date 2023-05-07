#include "lexer.h"

int ctor_lexer(lexer_struct* lexer_str_ptr) // CHECKED
{
    if(lexer_str_ptr == nullptr)
    {
        ERROR_MESSAGE(stderr, ERR_LEX_CALLOC_LEXER)
        return ERR_LEX_CALLOC_LEXER;
    }

    lexer_str_ptr->cur_pos_buff = 0;
    LEX_ERROR                   = LEXER_OK;   
    lexer_str_ptr->buff_ptr     = nullptr;
    LEX_TOKS                    = nullptr;
    lexer_str_ptr->buff_size    = 0;
    lexer_str_ptr->num_of_toks  = 10;
    lexer_str_ptr->cur_tok      = 0;

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
        LEX_TOKS[i].token_type = Empty;
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
    lexer_str_ptr->cur_tok      = LEX_POISON;

    return LEX_RET_OK;
}

int get_string(lexer_struct* lexer_str_ptr, char* file_name) // 
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
        if(isspace(STRING[POSITION]))
        {
            POSITION++;
            continue;
        }
        if(isalpha(STRING[POSITION]) || STRING[POSITION] == '_')
        {
            get_word(lexer_str_ptr);
            if(LEX_ERROR != LEXER_OK)
            {
                ERROR_MESSAGE(stderr, LEX_ERROR)
                return LEX_ERROR;
            }
        }
        // if(ispunct(STRING[POSITION]))
        // {

        // }
        printf("%c ", STRING[POSITION++]);
    }
}

int get_word(lexer_struct* lexer_str_ptr)
{
    size_t tok_text_pos = 0;
    while(isalpha(STRING[POSITION]) || (STRING[POSITION] == '_' && tok_text_pos == 4))
    {
        if(tok_text_pos == MAX_LEN_TOK_TEXT - 1)
        {
            LEX_ERROR = ERR_LEX_MAX_LEN_TOK;
            ERROR_MESSAGE(stderr, LEX_ERROR)
            return LEX_ERROR;
        }
        LEX_TOKS[CUR_TOK].token_text[tok_text_pos] = STRING[POSITION];
        tok_text_pos++;
        POSITION++;
    }

    if(isdigit(STRING[POSITION]) || STRING[POSITION] == '.' || STRING[POSITION] == '_')
    {
        LEX_ERROR = ERR_LEX_INV_WORD_NAME;
        ERROR_MESSAGE(stderr, ERR_LEX_INV_WORD_NAME)
        return LEX_ERROR;
    }

    if(!(strcmp(LEX_TOKS[CUR_TOK].token_text, "return")))
    {
        LEX_TOKS[CUR_TOK].token_type = Return;
    }
    else if(!(strcmp(LEX_TOKS[CUR_TOK].token_text, "var")))
    {
        LEX_TOKS[CUR_TOK].token_type = Var;
    }
    else
    {
        LEX_TOKS[CUR_TOK].token_type = Word;
    }
    LEX_TOKS[CUR_TOK].token_value.int_val = 0;
    CUR_TOK++;

    return LEX_RET_OK;
}

int get_op(lexer_struct* lexer_str_ptr)
{
    #define DEF_OP(op_name, op_code, op_text)                               \   
        case op_code:                                                       \
        {                                                                   \
            POSITION++;                                                     \
            LEX_TOKS[CUR_TOK].token_text[0] = op_text;                      \ 
            LEX_TOKS[CUR_TOK].token_text[1] = '\0';                         \
            LEX_TOKS[CUR_TOK].toke;                                         \
            return OP_FOUND;                                                \
        }                                                                   \                                                                   

    switch((int)STRING[POSITION])
    {
        #define DEF_OP(op_name, op_code, op_text) 
        #define DEF_FUNC(op_name, op_code, op_text) 
        #define DEF_LOGIC(op_name, op_code, op_text)
        #define DEF_BRACK(op_name, op_code, op_text) 
        #define DEF_KEY_W(op_name, op_code, op_text) 
        #include "def_cmd.h"
        #undef DEF_OP
        #undef DEF_FUNC
        #undef DEF_LOGIC
        #undef DEF_BRACK
        #undef DEF_KEY_W
        
        LEX_TOKS[CUR_TOK].token_type = Add;

        default:
            return OP_NOT_FOUND;
    }
}


// int realloc_toks()
// {




// }

void print_toks(lexer_struct* lexer_str_ptr) // CHECKED
{
    for(size_t i = 0; i < lexer_str_ptr->num_of_toks; i++)
    {
        printf("\n----------------------TOKEN %ld----------------------\n", i);
        printf("Token text: %s\n", LEX_TOKS[i].token_text);

        switch(LEX_TOKS[i].token_type)
        {
            case Add:
                printf("Token type: %d (%s)\n", Add, "Add"); 
                break;
            case Sub:
                printf("Token type: %d (%s)\n", Sub, "Sub"); 
                break;
            case Mul:
                printf("Token type: %d (%s)\n", Add, "Mul"); 
                break;
            case Div:
                printf("Token type: %d (%s)\n", Add, "Div"); 
                break;
            case Brack_l:
                printf("Token type: %d (%s)\n", Brack_l, "Brack_l"); 
                break;
            case Brack_r:
                printf("Token type: %d (%s)\n", Brack_r, "Brack_r"); 
                break;
            case Sqr_brack_l:
                printf("Token type: %d (%s)\n", Sqr_brack_l, "Sqr_brack_l"); 
                break;            
            case Sqr_brack_r:
                printf("Token type: %d (%s)\n", Sqr_brack_r, "Sqr_brack_r"); 
                break;
            case Fig_brack_l:
                printf("Token type: %d (%s)\n", Fig_brack_l, "Fig_brack_l"); 
                break;
            case Fig_brack_r:
                printf("Token type: %d (%s)\n", Fig_brack_r, "Fig_brack_r"); 
                break;
            case End_line:
                printf("Token type: %d (%s)\n", End_line, "End_line"); 
                break;
            case Pow:
                printf("Token type: %d (%s)\n", Pow, "Pow"); 
                break;            
            case Comma:
                printf("Token type: %d (%s)\n", Comma, "Comma"); 
                break;
            case Var:
                printf("Token type: %d (%s)\n", Var, "Var"); 
                break;
            case Val:
                printf("Token type: %d (%s)\n", Val, "Val"); 
                break;
            case Empty:
                printf("Token type: %d (%s)\n", Empty, "Empty"); 
                break;
            case Word:
                printf("Token type: %d (%s)\n", Word, "Word"); 
                break;            
            case Return:
                printf("Token type: %d (%s)\n", Return, "Return"); 
                break;
            default:
                ERROR_MESSAGE(stderr, ERR_LEX_NEW_TOK_TYPE)
                printf("Token type: %d (%s)\n", LEX_TOKS[i].token_type, "NEW_TOK_TYPE");
                break;
        }

        printf("Token int_val: %d\n", LEX_TOKS[i].token_value.int_val);
        printf("Token flt_val: %d\n", LEX_TOKS[i].token_value.flt_val);
        printf("----------------------TOKEN %ld----------------------\n", i);
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
