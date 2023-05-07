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
        ERROR_MESSAGE(stderr, ERR_LEX_CALLOC_TOK_ARR)
        return ERR_LEX_CALLOC_TOK_ARR;
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
        ERROR_MESSAGE(stderr, ERR_LEX_OPEN_INP_FILE)
        return ERR_LEX_OPEN_INP_FILE;
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
        ERROR_MESSAGE(stderr, ERR_LEX_CLOSE_INP_FILE)
        return ERR_LEX_CLOSE_INP_FILE;
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
        ERROR_MESSAGE(stderr, ERR_LEX_EMPTY_FILE)
        return ERR_LEX_EMPTY_FILE;
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
        ERROR_MESSAGE(stderr, ERR_LEX_CALLOC_BUFF)
        return ERR_LEX_CALLOC_BUFF;
    }

    int num_read = fread(lexer_str_ptr->buff_ptr, sizeof(char), lexer_str_ptr->buff_size, file_inp_ptr); // Reads the file into the buffer
    
    if((num_read <= 0) && (num_read > lexer_str_ptr->buff_size))
    {
        LEX_ERROR = ERR_LEX_READ_INTO_BUFF;
        ERROR_MESSAGE(stderr, ERR_LEX_READ_INTO_BUFF)
        return ERR_LEX_READ_INTO_BUFF;       
    }

    lexer_str_ptr->buff_ptr[lexer_str_ptr->buff_size] = '\0'; // Makes form the file null-terminated string
    fseek(file_inp_ptr, 0, SEEK_SET); // Puts the pointer inside the file to the start
    
    return LEX_RET_OK;
}

int get_toks(lexer_struct* lexer_str_ptr)
{
    while(POSITION != lexer_str_ptr->buff_size - 1)
    {
        realloc_toks(lexer_str_ptr);
        if(LEX_ERROR != LEXER_OK)
        {
            ERROR_MESSAGE(stderr, LEX_ERROR)
            return LEX_ERROR;
        }

        if(isspace(STRING[POSITION]) || STRING[POSITION] == '\r' || STRING[POSITION] == '\n')
        {
            POSITION++;
            continue;
        }
        else if(isalpha(STRING[POSITION]) || STRING[POSITION] == '_')
        {
            get_word(lexer_str_ptr);
            if(LEX_ERROR != LEXER_OK)
            {
                ERROR_MESSAGE(stderr, LEX_ERROR)
                return LEX_ERROR;
            }
            continue;
        }
        else if(ispunct(STRING[POSITION]))
        {
            get_op(lexer_str_ptr);
            if(LEX_ERROR != LEXER_OK)
            {
                ERROR_MESSAGE(stderr, LEX_ERROR)
                return LEX_ERROR;
            }
            continue;
        }
        get_val(lexer_str_ptr);
    }
    if(STRING[POSITION] != '$')
    {
        LEX_ERROR = ERR_LEX_NO_END_LINE;
        ERROR_MESSAGE(stderr, ERR_LEX_NO_END_LINE)
        return ERR_LEX_NO_END_LINE;
    }
    LEX_TOKS[CUR_TOK].token_text[0] = '$';
    LEX_TOKS[CUR_TOK].token_text[1] = '\0';
    LEX_TOKS[CUR_TOK].token_type = End_line;
    LEX_TOKS[CUR_TOK].token_value.int_val = End_line;
    CUR_TOK++;
}

int get_word(lexer_struct* lexer_str_ptr) // CHECKED
{
    size_t tok_text_pos = 0;
    while(isalpha(STRING[POSITION]) || (STRING[POSITION] == '_' && tok_text_pos == 4))
    {
        if(tok_text_pos == MAX_LEN_TOK_TEXT - 1)
        {
            LEX_ERROR = ERR_LEX_MAX_LEN_TOK;
            ERROR_MESSAGE(stderr, ERR_LEX_MAX_LEN_TOK)
            return ERR_LEX_MAX_LEN_TOK;
        }
        LEX_TOKS[CUR_TOK].token_text[tok_text_pos] = STRING[POSITION];
        tok_text_pos++;
        POSITION++;
    }

    if(isdigit(STRING[POSITION]) || STRING[POSITION] == '.' || STRING[POSITION] == '_')
    {
        LEX_ERROR = ERR_LEX_INV_WORD_NAME;
        ERROR_MESSAGE(stderr, ERR_LEX_INV_WORD_NAME)
        return ERR_LEX_INV_WORD_NAME;
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

int get_op(lexer_struct* lexer_str_ptr) // CHECKED
{
    switch((int)STRING[POSITION])
    {
        case Add:
            LEX_TOKS[CUR_TOK].token_text[0] = '+';
            LEX_TOKS[CUR_TOK].token_text[1] = '\0';
            LEX_TOKS[CUR_TOK].token_type = Add;
            LEX_TOKS[CUR_TOK].token_value.int_val = Add;
            POSITION++;
            CUR_TOK++;
            break;
        case Sub:
            LEX_TOKS[CUR_TOK].token_text[0] = '-';
            LEX_TOKS[CUR_TOK].token_text[1] = '\0';
            LEX_TOKS[CUR_TOK].token_type = Sub;
            LEX_TOKS[CUR_TOK].token_value.int_val = Sub;
            POSITION++;
            CUR_TOK++;
            break;
        case Div:
            LEX_TOKS[CUR_TOK].token_text[0] = '\\';
            LEX_TOKS[CUR_TOK].token_text[1] = '\0';
            LEX_TOKS[CUR_TOK].token_type = Div;
            LEX_TOKS[CUR_TOK].token_value.int_val = Div;
            POSITION++;
            CUR_TOK++;
            break;
        case Mul:
            LEX_TOKS[CUR_TOK].token_text[0] = '*';
            LEX_TOKS[CUR_TOK].token_text[1] = '\0';
            LEX_TOKS[CUR_TOK].token_type = Mul;
            LEX_TOKS[CUR_TOK].token_value.int_val = Mul;
            POSITION++;
            CUR_TOK++;
            break;
        case Brack_l:
            LEX_TOKS[CUR_TOK].token_text[0] = '(';
            LEX_TOKS[CUR_TOK].token_text[1] = '\0';
            LEX_TOKS[CUR_TOK].token_type = Brack_l;
            LEX_TOKS[CUR_TOK].token_value.int_val = Brack_l;
            POSITION++;
            CUR_TOK++;
            break;
        case Brack_r:
            LEX_TOKS[CUR_TOK].token_text[0] = ')';
            LEX_TOKS[CUR_TOK].token_text[1] = '\0';
            LEX_TOKS[CUR_TOK].token_type = Brack_r;
            LEX_TOKS[CUR_TOK].token_value.int_val = Brack_r;
            POSITION++;
            CUR_TOK++;
            break;
        case Sqr_brack_l:
            LEX_TOKS[CUR_TOK].token_text[0] = '[';
            LEX_TOKS[CUR_TOK].token_text[1] = '\0';
            LEX_TOKS[CUR_TOK].token_type = Sqr_brack_l;
            LEX_TOKS[CUR_TOK].token_value.int_val = Sqr_brack_l;
            POSITION++;
            CUR_TOK++;
            break;
        case Sqr_brack_r:
            LEX_TOKS[CUR_TOK].token_text[0] = ']';
            LEX_TOKS[CUR_TOK].token_text[1] = '\0';
            LEX_TOKS[CUR_TOK].token_type = Sqr_brack_r;
            LEX_TOKS[CUR_TOK].token_value.int_val = Sqr_brack_r;
            POSITION++;
            CUR_TOK++;
            break;
        case Fig_brack_l:
            LEX_TOKS[CUR_TOK].token_text[0] = '{';
            LEX_TOKS[CUR_TOK].token_text[1] = '\0';
            LEX_TOKS[CUR_TOK].token_type = Fig_brack_l;
            LEX_TOKS[CUR_TOK].token_value.int_val = Fig_brack_l;
            POSITION++;
            CUR_TOK++;
            break;
        case Fig_brack_r:
            LEX_TOKS[CUR_TOK].token_text[0] = '}';
            LEX_TOKS[CUR_TOK].token_text[1] = '\0';
            LEX_TOKS[CUR_TOK].token_type = Fig_brack_r;
            LEX_TOKS[CUR_TOK].token_value.int_val = Fig_brack_r;
            POSITION++;
            CUR_TOK++;
            break;
        case End_line:
            LEX_TOKS[CUR_TOK].token_text[0] = '$';
            LEX_TOKS[CUR_TOK].token_text[1] = '\0';
            LEX_TOKS[CUR_TOK].token_type = End_line;
            LEX_TOKS[CUR_TOK].token_value.int_val = End_line;
            POSITION++;
            CUR_TOK++;
            break;
        case Comma:
            LEX_TOKS[CUR_TOK].token_text[0] = ',';
            LEX_TOKS[CUR_TOK].token_text[1] = '\0';
            LEX_TOKS[CUR_TOK].token_type = Comma;
            LEX_TOKS[CUR_TOK].token_value.int_val = Comma;
            POSITION++;
            CUR_TOK++;
            break;
        case Eq:
            LEX_TOKS[CUR_TOK].token_text[0] = '=';
            LEX_TOKS[CUR_TOK].token_text[1] = '\0';
            LEX_TOKS[CUR_TOK].token_type = Eq;
            LEX_TOKS[CUR_TOK].token_value.int_val = Eq;
            POSITION++;
            CUR_TOK++;
            break;
        case Pow:
            LEX_TOKS[CUR_TOK].token_text[0] = '^';
            LEX_TOKS[CUR_TOK].token_text[1] = '\0';
            LEX_TOKS[CUR_TOK].token_type = Pow;
            LEX_TOKS[CUR_TOK].token_value.int_val = Pow;
            POSITION++;
            CUR_TOK++;
            break;
        default:
            ERROR_MESSAGE(stderr, ERR_LEX_INVALID_OP)
            LEX_ERROR = ERR_LEX_INVALID_OP;
            return ERR_LEX_INVALID_OP;
    }
    return LEX_RET_OK;
}

int get_val(lexer_struct* lexer_str_ptr) // CHECKED
{
    size_t length_val = 0;
    size_t num_points = 0;
    size_t length_tail = 0;
    float val = atof(&(STRING[POSITION]));
        
    while(('0' <= STRING[POSITION] && STRING[POSITION] <= '9') || (STRING[POSITION] == '-' && length_val == 0) 
        ||STRING[POSITION] == '.')
    {

        LEX_TOKS[CUR_TOK].token_text[length_val] = STRING[POSITION];
        if(num_points == 1)
        {
            length_tail++;
        }
        if(length_tail > 6)
        {
            LEX_ERROR = ERR_LEX_BIG_LENGTH_VAL;
            ERROR_MESSAGE(stderr, ERR_LEX_BIG_LENGTH_VAL)
            return ERR_LEX_BIG_LENGTH_VAL;
        }
        if(num_points > 1)
        {   
            LEX_ERROR = ERR_LEX_INVALID_VAL;
            ERROR_MESSAGE(stderr, ERR_LEX_INVALID_VAL)
            return ERR_LEX_INVALID_VAL;
        }
        if(STRING[POSITION] == '.')
        {
            num_points++;
        }
        length_val++;
        POSITION++;
    }
    LEX_TOKS[CUR_TOK].token_text[length_val] = '\0';
    if((check_op(STRING[POSITION]) == ERR_LEX_INVALID_OP) && isspace(STRING[POSITION]) == 0)
    {   
        LEX_ERROR = ERR_LEX_INVALID_VAL;
        ERROR_MESSAGE(stderr, ERR_LEX_INVALID_VAL)
        return ERR_LEX_INVALID_VAL;
    }

    LEX_TOKS[CUR_TOK].token_type = Val;
    LEX_TOKS[CUR_TOK].token_value.flt_val = val;
    CUR_TOK++;
    return LEX_RET_OK;
}

int check_op(char op_char)
{
    if(op_char == '+')
    {
        return Add;
    }
    else if(op_char == '-')
    {
        return Sub;
    }
    else if(op_char == '*')
    {
        return Mul;
    }
    else if(op_char == '\\')
    {
        return Div;
    }
    else if(op_char == ',')
    {
        return Comma;
    }
    else if(op_char == '^')
    {
        return Pow;
    }
    else if(op_char == '$')
    {
        return End_line;
    }
    else if(op_char == ')')
    {
        return Brack_r;
    }
    else if(op_char == '\n')
    {
        return New_line_n;
    }
    else if(op_char == '\r')
    {
        return New_line_r;
    }
    return ERR_LEX_INVALID_OP;
}

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
            case Eq:
                printf("Token type: %d (%s)\n", Eq, "Eq"); 
                break;
            default:
                ERROR_MESSAGE(stderr, ERR_LEX_NEW_TOK_TYPE)
                printf("Token type: %d (%s)\n", LEX_TOKS[i].token_type, "NEW_TOK_TYPE");
                break;
        }

        printf("Token int_val: %d\n", LEX_TOKS[i].token_value.int_val);
        printf("Token flt_val: %f\n", LEX_TOKS[i].token_value.flt_val);
        printf("----------------------TOKEN %ld----------------------\n", i);
    }
}

int realloc_toks(lexer_struct* lexer_str_ptr)
{
    if(lexer_str_ptr->cur_tok == lexer_str_ptr->num_of_toks)
    {
        lexer_str_ptr->num_of_toks *= 2;
        LEX_TOKS = (token*)realloc(LEX_TOKS, lexer_str_ptr->num_of_toks * sizeof(token));

        if(LEX_TOKS == nullptr)
        {
            ERROR_MESSAGE(stderr, ERR_LEX_REALLOC_TOKS)
            LEX_ERROR = ERR_LEX_REALLOC_TOKS;
            return ERR_LEX_REALLOC_TOKS;
        }

        for(size_t i = CUR_TOK; i < lexer_str_ptr->num_of_toks; i++)
        {
            stpcpy(LEX_TOKS[i].token_text, "");
            LEX_TOKS[i].token_type = Empty;
            LEX_TOKS[i].token_value.flt_val = 0;
        }
    }
    return LEX_RET_OK;
}
