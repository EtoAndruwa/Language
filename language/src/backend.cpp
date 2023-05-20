#include "language.h"

int ctor_backend(Backend_struct* backend_str_ptr) // CHECKED
{
    VAR_NUM     = 5;
    FUNC_NUM    = 5;
    CUR_RAM_ID  = 0;
    CUR_FUNC_ID = 0;
    CUR_VAR_ID  = 0;
    CUR_DECL_ID = 0;
    DECL_NUM    = 5;
    backend_str_ptr->main_node_id = 0;
    FLAG_ID     = 1;

    backend_str_ptr->funcs = (func_info*)calloc(FUNC_NUM, sizeof(func_info));
    if(backend_str_ptr->funcs == nullptr)
    {
        BACK_ERROR = ERR_BCK_CALLOC_FUNCS;
        ERROR_MESSAGE(stderr, ERR_BCK_CALLOC_FUNCS)
        return ERR_BCK_CALLOC_FUNCS;
    }

    for(size_t i = 0; i < VAR_NUM; i++)
    {
        strcpy(FUNCS_ARR[i].func_name, "EMPTY");
        FUNCS_ARR[i].num_of_vars = -1;
    }

    backend_str_ptr->vars = (var_info*)calloc(VAR_NUM, sizeof(var_info));
    if(backend_str_ptr->vars == nullptr)
    {
        BACK_ERROR = ERR_BCK_CALLOC_VARS;
        ERROR_MESSAGE(stderr, ERR_BCK_CALLOC_VARS)
        return ERR_BCK_CALLOC_VARS;
    }

    for(size_t i = 0; i < VAR_NUM; i++)
    {
        strcpy(VARS_ARR[i].var_text, "EMPTY");
        strcpy(VARS_ARR[i].name_parent_func, "EMPTY");
        VARS_ARR[i].var_ram_id = -1;
    }
    
    backend_str_ptr->decl_nodes = (Node**)calloc(DECL_NUM, sizeof(Node*));
    if(backend_str_ptr->decl_nodes == nullptr)
    {
        BACK_ERROR = ERR_BCK_CALLOC_DECL_NODS;
        ERROR_MESSAGE(stderr, ERR_BCK_CALLOC_DECL_NODS)
        return ERR_BCK_CALLOC_DECL_NODS;
    }

    for(size_t i = 0; i < DECL_NUM; i++)
    {
        backend_str_ptr->decl_nodes[i] = nullptr;
    }

    BACK_ERROR = BACK_OK;
    return BACK_OK;
}

int dtor_backend(Backend_struct* backend_str_ptr) // CHECKED
{
    for(size_t i = 0; i < FUNC_NUM; i++)
    {
        backend_str_ptr->funcs[i].num_of_vars = LEX_POISON;
        strcpy(backend_str_ptr->funcs[i].func_name, "POISON");
    }

    free(backend_str_ptr->funcs);
    backend_str_ptr->funcs = nullptr;

    for(size_t i = 0; i < VAR_NUM; i++)
    {
        backend_str_ptr->vars[i].var_ram_id = LEX_POISON;
        strcpy(backend_str_ptr->vars[i].var_text, "POISON");
        strcpy(backend_str_ptr->vars[i].name_parent_func, "POISON");
    }

    free(backend_str_ptr->vars);
    backend_str_ptr->vars = nullptr;

    for(size_t i = 0; i < DECL_NUM; i++)
    {
        backend_str_ptr->decl_nodes[i] = nullptr;
    }

    free(backend_str_ptr->decl_nodes);
    backend_str_ptr->decl_nodes = nullptr;

    backend_str_ptr->num_of_decls = LEX_POISON;
    backend_str_ptr->num_of_funcs = LEX_POISON;
    backend_str_ptr->num_of_vars  = LEX_POISON;

    backend_str_ptr->cur_decl_id  = LEX_POISON;
    backend_str_ptr->cur_func_id  = LEX_POISON;
    backend_str_ptr->cur_ram_id   = LEX_POISON;
    backend_str_ptr->cur_var_id   = LEX_POISON;
    backend_str_ptr->cur_flag_id  = LEX_POISON;

    backend_str_ptr->error_code   = LEX_POISON;
    backend_str_ptr->main_node_id = LEX_POISON;

    return BACK_OK;
}

void print_vars(Backend_struct* backend_str_ptr) // CHECKED
{
    printf("\n--------------------VARS--------------------\n");
    for(size_t i = 0; i < VAR_NUM; i++)
    {
        printf("VARS_ARR[%ld].var_text = %s, VARS_ARR[%ld].var_ram_id = %ld, VARS_ARR[%ld].name_parent_func = %s\n", i, VARS_ARR[i].var_text, i, VARS_ARR[i].var_ram_id, i, VARS_ARR[i].name_parent_func);
    }
    printf("--------------------VARS--------------------\n");
}

void print_funcs(Backend_struct* backend_str_ptr) // CHECKED
{
    printf("\n--------------------FUNCS--------------------\n");
    for(size_t i = 0; i < FUNC_NUM; i++)
    {
        printf("FUNC[%ld].name = %s, FUNC[%ld].num_of_vars = %ld\n", i, FUNCS_ARR[i].func_name, i, FUNCS_ARR[i].num_of_vars);
    }
    printf("--------------------FUNCS--------------------\n");
}

int create_asm(Backend_struct* backend_str_ptr) // CHECKED
{
    FILE* asm_file_ptr = fopen(FILE_ASM_NAME, "w");
    if(asm_file_ptr == nullptr)
    {
        ERROR_MESSAGE(stderr, ERR_BCK_OPEN_ASM_FILE)
        BACK_ERROR = ERR_BCK_OPEN_ASM_FILE;
        return ERR_BCK_OPEN_ASM_FILE;
    }

    if(translate_expr(backend_str_ptr, DECL_NODES[backend_str_ptr->main_node_id]->left_child, asm_file_ptr, MAIN_LANG_DEF) != BACK_OK)
    {
        ERROR_MESSAGE(stderr, ERR_BCK_TRANSLATE_MAIN)
        return ERR_BCK_TRANSLATE_MAIN;
    }
    fprintf(asm_file_ptr, "HLT\n");


    for(size_t i = 0; i < DECL_NUM; i++)
    {
        if(DECL_NODES[i] != nullptr)
        {
            if(backend_str_ptr->main_node_id == i)
            {
                continue;
            }

            if(print_decl_funcs(backend_str_ptr, DECL_NODES[i], asm_file_ptr, DECL_NODES[i]->left_child->left_child->value.text) != BACK_OK)
            {
                ERROR_MESSAGE(stderr, ERR_BCK_TRANSLATE_FUNC_DECL)
                BACK_ERROR = ERR_BCK_TRANSLATE_FUNC_DECL;
                return ERR_BCK_TRANSLATE_FUNC_DECL;
            }
        }
        break;
    }

    if(fclose(asm_file_ptr) == EOF)
    {
        ERROR_MESSAGE(stderr, ERR_BCK_CLOSE_ASM_FILE)
        BACK_ERROR = ERR_BCK_CLOSE_ASM_FILE;
        return ERR_BCK_CLOSE_ASM_FILE;
    }
    return BACK_OK;
}

int find_main_node(Backend_struct* backend_str_ptr, Node* node_ptr) // CHECKED finds the main and decl head nodes in the tree form expr_head nodes
{
    if(node_ptr == nullptr)
    {
        return BACK_OK;
    }

    if(node_ptr->type == EXPR_HEAD)
    {
        if(realloc_decls(backend_str_ptr) != BACK_OK)
        {
            ERROR_MESSAGE(stderr, ERR_BCK_REALLOC_DECLS);
            return ERR_BCK_REALLOC_DECLS;
        }

        if(NODE_LEFT_CHILD->type == MAIN)
        {
            DECL_NODES[CUR_DECL_ID] = NODE_LEFT_CHILD;
            backend_str_ptr->main_node_id = CUR_DECL_ID;
            CUR_DECL_ID++; 
        }

        if(NODE_LEFT_CHILD->type == DECL_FUNC_HEAD)
        {
            for(size_t i = 0; i < DECL_NUM; i++)
            {
                if(!strcmp(NODE_LEFT_CHILD->left_child->left_child->value.text, FUNCS_ARR[i].func_name))
                {
                    ERROR_MESSAGE(stderr, ERR_BCK_FUNC_REDECL)
                    BACK_ERROR = ERR_BCK_FUNC_REDECL;
                    return ERR_BCK_FUNC_REDECL;
                }
            }

            strcpy(FUNCS_ARR[CUR_FUNC_ID].func_name, NODE_LEFT_CHILD->left_child->left_child->value.text);

            Node* args_node_ptr = NODE_LEFT_CHILD->left_child->right_child->left_child;
            size_t count_vars = 0;

            while(args_node_ptr != nullptr)
            {
                count_vars++;
                args_node_ptr = args_node_ptr->right_child;
            }
            FUNCS_ARR[CUR_FUNC_ID].num_of_vars = count_vars;

            CUR_FUNC_ID++;
            DECL_NODES[CUR_DECL_ID] = NODE_LEFT_CHILD;
            CUR_DECL_ID++;
        }
        
        return find_main_node(backend_str_ptr, NODE_RIGHT_CHILD);
    }
}

void print_decls(Backend_struct* backend_str_ptr) // CHECKED
{
    printf("\n--------------------DECLS--------------------\n");
    for(size_t i = 0; i < DECL_NUM; i++)
    {
        printf("backend_str_ptr->decl_nodes[%ld] = %p\n", i, backend_str_ptr->decl_nodes[i]);
    }
    printf("--------------------DECLS--------------------\n");
}

int translate_expr(Backend_struct* backend_str_ptr, Node* node_ptr, FILE* asm_file_ptr, char* func_name, int jmp_for_break) // CHECKED
{
    if(node_ptr == nullptr)
    {
        return BACK_OK;
    }

    if(node_ptr->type == EXPR_HEAD)
    {
        if(NODE_LEFT_CHILD->type == OP_HEAD)
        {
            if(translate_var_decl(backend_str_ptr, NODE_LEFT_CHILD->left_child, asm_file_ptr, func_name) != BACK_OK)
            {
                ERROR_MESSAGE(stderr, ERR_BCK_TRANSLATE_VAR_DECL)
                return ERR_BCK_TRANSLATE_VAR_DECL;
            }
        }
        else if(NODE_LEFT_CHILD->type == RETURN && !strcmp(func_name, MAIN_LANG_DEF))
        {
            if(NODE_LEFT_CHILD->left_child->left_child->type != VAL_HEAD || check_is_positive(NODE_LEFT_CHILD->left_child->left_child->left_child->value.node_value) != IS_ZERO)
            {
                ERROR_MESSAGE(stderr, ERR_BKC_INV_RETURN_MAIN)
                return ERR_BKC_INV_RETURN_MAIN;
            }
        }
        else if(NODE_LEFT_CHILD->type == RETURN && strcmp(func_name, MAIN_LANG_DEF))
        {
            if(print_sub_eq(backend_str_ptr, NODE_LEFT_CHILD->left_child->left_child, asm_file_ptr, func_name) != BACK_OK)
            {
                ERROR_MESSAGE(stderr, ERR_BCK_TRANSLATE_SUB_EQ)
                return ERR_BCK_TRANSLATE_SUB_EQ;
            }
            fprintf(asm_file_ptr, "POP ax\n");
            fprintf(asm_file_ptr, "RET\n");
        }
        else if(NODE_LEFT_CHILD->type == BREAK)
        {
            if(jmp_for_break != -1)
            {
                fprintf(asm_file_ptr, "JMP :%d\n", jmp_for_break);
            }
            else
            {
                ERROR_MESSAGE(stderr, ERR_BCK_TRANSLATE_SUB_EQ)
                return ERR_BCK_TRANSLATE_SUB_EQ;
            }
        }
        else if(NODE_LEFT_CHILD->type == LOGIC_OP_HEAD)
        {
            print_logic(backend_str_ptr, NODE_LEFT_CHILD->left_child, asm_file_ptr, func_name, jmp_for_break);
        }
        else if(NODE_LEFT_CHILD->type == FUNC_CALL)
        {
            print_lib_funcs(backend_str_ptr, NODE_LEFT_CHILD->left_child, asm_file_ptr, func_name);
        }         

        if(translate_expr(backend_str_ptr, NODE_RIGHT_CHILD, asm_file_ptr, func_name, jmp_for_break) != BACK_OK)
        {
            ERROR_MESSAGE(stderr, ERR_BCK_TRANSLATE_EXPR)
            return ERR_BCK_TRANSLATE_EXPR;
        }
        return BACK_OK;
    }

    printf("node_type %ld, %p\n", node_ptr->type, node_ptr);
    ERROR_MESSAGE(stderr, ERR_BCK_NEW_TYPE_EXPR)
    BACK_ERROR = ERR_BCK_NEW_TYPE_EXPR;
    return ERR_BCK_NEW_TYPE_EXPR;
}

int translate_var_decl(Backend_struct* backend_str_ptr, Node* node_ptr, FILE* asm_file_ptr, char* func_name, int decl_flag) // CHECKED
{
    if(realloc_vars(backend_str_ptr) != BACK_OK)
    {
        ERROR_MESSAGE(stderr, ERR_BCK_REALLOC_VARS);
        return ERR_BCK_REALLOC_VARS;
    }

    if(NODE_LEFT_CHILD->type == DECL_VAR_HEAD)
    {
        if(decl_flag == NOT_FUNC_ARGS)
        {
            if(print_sub_eq(backend_str_ptr, NODE_RIGHT_CHILD, asm_file_ptr, func_name) != BACK_OK)
            {
                ERROR_MESSAGE(stderr, BACK_ERROR)
                BACK_ERROR = DECL_VAR_HEAD;
                return BACK_ERROR;
            }
            fprintf(asm_file_ptr, "POP [%ld]\n\n", CUR_RAM_ID);
        }

        for(size_t i = 0; i < CUR_VAR_ID; i++)
        {
            if(!strcmp(VARS_ARR[i].var_text, NODE_LEFT_CHILD->left_child->left_child->value.text) && !(strcmp(VARS_ARR[i].name_parent_func, func_name)))
            {
                ERROR_MESSAGE(stderr, ERR_BCK_VAR_REDECL)
                BACK_ERROR = ERR_BCK_VAR_REDECL;
                return ERR_BCK_VAR_REDECL;
            }
        }

        strcpy(VARS_ARR[CUR_VAR_ID].var_text, NODE_LEFT_CHILD->left_child->left_child->value.text);
        strcpy(VARS_ARR[CUR_VAR_ID].name_parent_func, func_name);
        VARS_ARR[CUR_VAR_ID].var_ram_id = CUR_RAM_ID;


        CUR_RAM_ID++; // empty index for the new var in the CPU's RAM
        CUR_VAR_ID++; // empty index in the inner var's array of backend
        return BACK_OK;
        
    }
    else if(NODE_LEFT_CHILD->type == VAR_HEAD)
    {
        if(translate_var_assign(backend_str_ptr, node_ptr, asm_file_ptr, func_name) != BACK_OK)
        {
            ERROR_MESSAGE(stderr, ERR_BCK_VAR_ASSIGN)
            BACK_ERROR = ERR_BCK_VAR_ASSIGN;
            return ERR_BCK_VAR_ASSIGN;
        }
        return BACK_OK;
    }

    BACK_ERROR = ERR_BCK_NEW_TYPE_VAR_DECL;
    ERROR_MESSAGE(stderr, ERR_BCK_NEW_TYPE_VAR_DECL)
    return ERR_BCK_NEW_TYPE_VAR_DECL;
}

int print_sub_eq(Backend_struct* backend_str_ptr, Node* node_ptr, FILE* asm_file_ptr, char* func_name) // CHECKED
{
    if(node_ptr == nullptr)
    {
        return BACK_OK;
    }

    if(node_ptr->type == VAL_HEAD)
    {
        fprintf(asm_file_ptr, "PUSH %f\n", NODE_LEFT_CHILD->value.node_value);
        return BACK_OK;
    }
    else if(node_ptr->type == VAR_HEAD)
    {
        for(size_t i = 0; i < VAR_NUM; i++)
        {
            if(VARS_ARR[i].var_ram_id == -1)
            {
                BACK_ERROR = ERR_BCK_FOUND_UNDECL_VAR;
                ERROR_MESSAGE(stderr, ERR_BCK_FOUND_UNDECL_VAR)
                return ERR_BCK_FOUND_UNDECL_VAR;
            }
        
            if(!strcmp(VARS_ARR[i].var_text, NODE_LEFT_CHILD->value.text) && !strcmp(VARS_ARR[i].name_parent_func, func_name))
            {
                fprintf(asm_file_ptr, "PUSH [%ld]\n", i);
                return BACK_OK;
            }
        }

        BACK_ERROR = ERR_BCK_FOUND_UNDECL_VAR;
        ERROR_MESSAGE(stderr, ERR_BCK_FOUND_UNDECL_VAR)
        return ERR_BCK_FOUND_UNDECL_VAR;
    }   
    else if(node_ptr->type == OP_HEAD)
    {
        if(print_sub_eq(backend_str_ptr, NODE_LEFT_CHILD, asm_file_ptr, func_name) != BACK_OK)
        {
            ERROR_MESSAGE(stderr, BACK_ERROR)
            return BACK_ERROR;
        }
        return BACK_OK;
    }
    else if(node_ptr->type == FUNC_CALL)
    {
        if(!strcmp(NODE_LEFT_CHILD->left_child->value.text, func_name) && (strcmp(NODE_LEFT_CHILD->left_child->value.text, MAIN_LANG_DEF) != 0))
        {
            size_t save_last_id = 0;
            size_t num_of_args  = 0;

            for(size_t i = 0; i < CUR_VAR_ID; i++)
            {
                if(!strcmp(VARS_ARR[i].name_parent_func, func_name))
                {
                    save_last_id = i;
                    num_of_args++;
                    fprintf(asm_file_ptr, "PUSH [%ld]\n", i);
                }
            }

            if(print_call_func(backend_str_ptr, NODE_LEFT_CHILD, asm_file_ptr, func_name) != BACK_OK)
            {
                ERROR_MESSAGE(stderr, BACK_ERROR)
                return BACK_ERROR;
            }

            for(size_t i = save_last_id; i >=  (save_last_id - (num_of_args -1)); i--)
            {
                fprintf(asm_file_ptr, "POP [%ld]\n", i);
            }

            fprintf(asm_file_ptr, "PUSH ax\n");

            return BACK_OK;
        }
        else if(!strcmp(NODE_LEFT_CHILD->left_child->value.text, MAIN_LANG_DEF))
        {
            printf("%s", func_name);
            ERROR_MESSAGE(stderr, ERR_BCK_MAIN_CANNOT_BE_CLLD)
            BACK_ERROR = ERR_BCK_MAIN_CANNOT_BE_CLLD;
            return ERR_BCK_MAIN_CANNOT_BE_CLLD;
        }
        else
        {
            if(print_call_func(backend_str_ptr, NODE_LEFT_CHILD, asm_file_ptr, func_name) != BACK_OK)
            {
                ERROR_MESSAGE(stderr, BACK_ERROR)
                return BACK_ERROR;
            }
            return BACK_OK;
        }
    }

    if(print_sub_eq(backend_str_ptr, NODE_LEFT_CHILD, asm_file_ptr, func_name) != BACK_OK)
    {
        ERROR_MESSAGE(stderr, BACK_ERROR)
        return BACK_ERROR;   
    }
    if(print_sub_eq(backend_str_ptr, NODE_RIGHT_CHILD, asm_file_ptr, func_name) != BACK_OK)
    {
        ERROR_MESSAGE(stderr, BACK_ERROR)
        return BACK_ERROR;   
    }
    
    switch(node_ptr->value.op_number)
    {
        case Add:
            fprintf(asm_file_ptr, "ADD\n");
            return BACK_OK;
        case Sub:
            fprintf(asm_file_ptr, "SUB\n");
            return BACK_OK;
        case Div:
            fprintf(asm_file_ptr, "DIV\n");
            return BACK_OK;
        case Mul:
            fprintf(asm_file_ptr, "MUL\n");
            return BACK_OK; 
        default:
            BACK_ERROR = ERR_BCK_FOUND_NEW_OP;
            ERROR_MESSAGE(stderr, ERR_BCK_FOUND_NEW_OP)
            fprintf(asm_file_ptr, "ERROR_OP\n");
            return ERR_BCK_FOUND_NEW_OP;
    }

    ERROR_MESSAGE(stderr, ERR_BCK_NEW_TYPE_SUB_EQ)
    BACK_ERROR = ERR_BCK_NEW_TYPE_SUB_EQ;
    return ERR_BCK_NEW_TYPE_SUB_EQ;
}

int translate_var_assign(Backend_struct* backend_str_ptr, Node* node_ptr, FILE* asm_file_ptr, char* func_name) // CHECKED
{
    int var_id = -1;  // flag for found_var_id

    for(size_t i = 0; i < VAR_NUM; i++)
    {
        if(VARS_ARR[i].var_ram_id == -1)
        {
            break;
        }
        if(!strcmp(VARS_ARR[i].var_text, NODE_LEFT_CHILD->left_child->value.text) && !strcmp(VARS_ARR[i].name_parent_func, func_name))
        {
            var_id = i;  
            break;
        }
    }

    if(var_id != -1)
    {
        if(print_sub_eq(backend_str_ptr, NODE_RIGHT_CHILD, asm_file_ptr, func_name) != BACK_OK)
        {
            ERROR_MESSAGE(stderr, BACK_ERROR)
            return BACK_ERROR;
        }

        fprintf(asm_file_ptr, "POP [%ld]\n\n", var_id);
        return BACK_OK;
    }

    ERROR_MESSAGE(stderr, ERR_BCK_FOUND_UNDECL_VAR)
    BACK_ERROR = ERR_BCK_FOUND_UNDECL_VAR;
    return ERR_BCK_FOUND_UNDECL_VAR;
}

int print_decl_funcs(Backend_struct* backend_str_ptr, Node* node_ptr, FILE* asm_file_ptr, char* func_name) // CHECKED
{
    if(node_ptr->type == DECL_FUNC_HEAD)
    {
        fprintf(asm_file_ptr, "\n\n%s:\n", NODE_LEFT_CHILD->left_child->value.text);

        Node* args = NODE_LEFT_CHILD->right_child->left_child;
        int svd_cur_ram_id = CUR_RAM_ID;

        while(args != nullptr)
        {
            translate_var_decl(backend_str_ptr, args->left_child->left_child, asm_file_ptr, func_name, DECL_FUNC_ARGS);
            args = args->right_child;
        }

        int num_of_args = -1;
        for(size_t i = 0; i < CUR_FUNC_ID; i ++)
        {
            if(!strcmp(FUNCS_ARR[i].func_name, func_name))
            {
                num_of_args = FUNCS_ARR[i].num_of_vars;
                break;
            }
        }

        if(num_of_args == -1)
        {
            ERROR_MESSAGE(stderr, ERR_BKC_FUNC_ARGS_NOT_FOUND)
            BACK_ERROR = ERR_BKC_FUNC_ARGS_NOT_FOUND;
            return ERR_BKC_FUNC_ARGS_NOT_FOUND;
        }

        for(size_t i = svd_cur_ram_id + (num_of_args - 1); i >= svd_cur_ram_id; i--)
        {
            fprintf(asm_file_ptr, "POP [%ld]\n", i);
        }
        fprintf(asm_file_ptr, "\n");

        translate_expr(backend_str_ptr, NODE_LEFT_CHILD->right_child->right_child, asm_file_ptr, func_name);

        return BACK_OK;
    }

    BACK_ERROR = ERR_BCK_NEW_TYPE_DECL_FUNC;
    ERROR_MESSAGE(stderr, ERR_BCK_NEW_TYPE_DECL_FUNC)
    return ERR_BCK_NEW_TYPE_DECL_FUNC;
}

int print_call_func(Backend_struct* backend_str_ptr, Node* node_ptr, FILE* asm_file_ptr, char* func_name) // CHECKED
{
    if(node_ptr->type == FUNC_HEAD)
    {
        Node* func_args_node = NODE_RIGHT_CHILD->left_child;
        size_t count_args = 0;

        while(func_args_node != nullptr)
        {
            print_sub_eq(backend_str_ptr, func_args_node->left_child, asm_file_ptr, func_name);
            func_args_node = func_args_node->right_child;
            count_args++;
        }

        for(size_t i = 0; i < CUR_FUNC_ID; i++)
        {
            if(!strcmp(NODE_LEFT_CHILD->value.text, FUNCS_ARR[i].func_name))
            {
                if(count_args != FUNCS_ARR[i].num_of_vars)
                {
                    ERROR_MESSAGE(stderr, ERR_BCK_INV_NUM_OF_ARGS)
                    BACK_ERROR = ERR_BCK_INV_NUM_OF_ARGS;
                    return ERR_BCK_INV_NUM_OF_ARGS;
                }
            }
        }

        fprintf(asm_file_ptr, "CALL %s:\n", NODE_LEFT_CHILD->value.text);
        if(strcmp(func_name, NODE_LEFT_CHILD->value.text) != 0)
        {
            fprintf(asm_file_ptr, "PUSH ax\n");
        }

        return BACK_OK;
    }

    ERROR_MESSAGE(stderr, ERR_BCK_NEW_TYPE_FUNC_CALL)
    BACK_ERROR = ERR_BCK_NEW_TYPE_FUNC_CALL;
    return ERR_BCK_NEW_TYPE_FUNC_CALL;
}

int realloc_vars(Backend_struct* backend_str_ptr) // CHECKED
{
    if(CUR_VAR_ID == VAR_NUM)
    {
        VAR_NUM *= 2;
        VARS_ARR = (var_info*)realloc(VARS_ARR, VAR_NUM * sizeof(var_info));
        
        if(VARS_ARR == nullptr)
        {
            ERROR_MESSAGE(stderr, ERR_BCK_REALLOC_VARS)
            BACK_ERROR = ERR_BCK_REALLOC_VARS;
            return ERR_BCK_REALLOC_VARS;
        }

        for(size_t i = CUR_VAR_ID; i < VAR_NUM; i++)
        {
            strcpy(VARS_ARR[i].name_parent_func, "EMPTY");
            strcpy(VARS_ARR[i].var_text, "EMPTY");
            VARS_ARR[i].var_ram_id = -1;
        }
    }
    return BACK_OK;
}

int realloc_funcs(Backend_struct* backend_str_ptr) // CHECKED
{
    if(CUR_FUNC_ID == FUNC_NUM)
    {
        FUNC_NUM *= 2;
        FUNCS_ARR = (func_info*)realloc(FUNCS_ARR, FUNC_NUM * sizeof(func_info));
        
        if(FUNCS_ARR == nullptr)
        {
            ERROR_MESSAGE(stderr, ERR_BCK_REALLOC_FUNCS)
            BACK_ERROR = ERR_BCK_REALLOC_FUNCS;
            return ERR_BCK_REALLOC_FUNCS;
        }

        for(size_t i = CUR_FUNC_ID; i < FUNC_NUM; i++)
        {
            strcpy(FUNCS_ARR[i].func_name, "EMPTY");
            FUNCS_ARR[i].num_of_vars = -1;
        }
    }
    return BACK_OK;
}

int realloc_decls(Backend_struct* backend_str_ptr) // CHECKED
{
    if(CUR_DECL_ID == DECL_NUM)
    {
        DECL_NUM *= 2;
        DECL_NODES = (Node**)realloc(DECL_NODES, DECL_NUM * sizeof(Node*));
        
        if(FUNCS_ARR == nullptr)
        {
            ERROR_MESSAGE(stderr, ERR_BCK_REALLOC_FUNCS)
            BACK_ERROR = ERR_BCK_REALLOC_FUNCS;
            return ERR_BCK_REALLOC_FUNCS;
        }

        for(size_t i = CUR_DECL_ID; i < DECL_NUM; i++)
        {
            DECL_NODES[i] = nullptr;
        }
    }
    return BACK_OK;
}

int print_logic(Backend_struct* backend_str_ptr, Node* node_ptr, FILE* asm_file_ptr, char* func_name, int jmp_for_break)
{
    if(node_ptr->value.op_number == If)
    {
        print_sub_eq(backend_str_ptr, NODE_LEFT_CHILD, asm_file_ptr, func_name);
        fprintf(asm_file_ptr, "POP ix\n");

        size_t save_cur_flag_1 = FLAG_ID; // allows inner ifs in the body of logic op
        size_t save_cur_flag_2 = FLAG_ID + 1;
        FLAG_ID += 2;

        fprintf(asm_file_ptr, "JZ :%ld\n", save_cur_flag_1);

        if(NODE_RIGHT_CHILD->right_child->type != EMPTY)
        {
            if(jmp_for_break != -1)
            {
                translate_expr(backend_str_ptr, NODE_RIGHT_CHILD->left_child, asm_file_ptr, func_name, jmp_for_break);
            }
            else
            {
                translate_expr(backend_str_ptr, NODE_RIGHT_CHILD->left_child, asm_file_ptr, func_name, save_cur_flag_2);
            }
        }
        fprintf(asm_file_ptr, "JMP :%ld\n", save_cur_flag_2);
        fprintf(asm_file_ptr, ":%ld\n", save_cur_flag_1);


        if(jmp_for_break != -1)
        {
            translate_expr(backend_str_ptr, NODE_RIGHT_CHILD->left_child, asm_file_ptr, func_name, jmp_for_break);
        }
        else
        {
            translate_expr(backend_str_ptr, NODE_RIGHT_CHILD->left_child, asm_file_ptr, func_name, save_cur_flag_2);
        }
        fprintf(asm_file_ptr, ":%ld\n", save_cur_flag_2);

        return BACK_OK;
    }
    else if(node_ptr->value.op_number == For)
    {
        translate_var_decl(backend_str_ptr, NODE_LEFT_CHILD->left_child->left_child, asm_file_ptr, func_name);

        size_t save_cur_flag_1 = FLAG_ID; // allows inner ifs in the body of logic op
        size_t save_cur_flag_2 = FLAG_ID + 1;
        FLAG_ID += 2;

        fprintf(asm_file_ptr, ":%ld\n", save_cur_flag_1);
        print_sub_eq(backend_str_ptr, NODE_LEFT_CHILD->right_child->left_child, asm_file_ptr, func_name);
        fprintf(asm_file_ptr, "POP ix\n");
        fprintf(asm_file_ptr, "JZ :%ld\n", save_cur_flag_2);

        translate_expr(backend_str_ptr, NODE_RIGHT_CHILD, asm_file_ptr, func_name, save_cur_flag_2);

        translate_var_assign(backend_str_ptr, NODE_LEFT_CHILD->right_child->right_child->left_child->left_child, asm_file_ptr, func_name);
        fprintf(asm_file_ptr, "JMP :%ld\n", save_cur_flag_1);
        fprintf(asm_file_ptr, ":%ld\n", save_cur_flag_2);

        return BACK_OK;
    }
    else if(node_ptr->value.op_number == While)
    {
        size_t save_cur_flag_1 = FLAG_ID; // allows inner ifs in the body of logic op
        size_t save_cur_flag_2 = FLAG_ID + 1;
        FLAG_ID += 2;

        fprintf(asm_file_ptr, "\n:%ld\n", save_cur_flag_1);
        print_sub_eq(backend_str_ptr, NODE_LEFT_CHILD->left_child, asm_file_ptr, func_name);
        fprintf(asm_file_ptr, "POP ix\n");
        fprintf(asm_file_ptr, "JZ :%ld\n", save_cur_flag_2);

        translate_expr(backend_str_ptr, NODE_RIGHT_CHILD, asm_file_ptr, func_name, save_cur_flag_2);

        fprintf(asm_file_ptr, "JMP :%ld\n", save_cur_flag_1);
        fprintf(asm_file_ptr, ":%ld\n\n", save_cur_flag_2);

        return BACK_OK;
    }
}

int print_lib_funcs(Backend_struct* backend_str_ptr, Node* node_ptr, FILE* asm_file_ptr, char* func_name)
{
    printf("HERE!\n");
    if(node_ptr->type == FUNC_HEAD && (!(strcmp(NODE_LEFT_CHILD->value.text, SCANF_LANG_DEF)) || !(strcmp(NODE_LEFT_CHILD->value.text, PRINTF_LANG_DEF))))
    {
        Node* args = NODE_RIGHT_CHILD->left_child;
        size_t num_of_args = count_func_args(args);

        if(!(strcmp(NODE_LEFT_CHILD->value.text, SCANF_LANG_DEF)))
        {
            if(check_func_args(backend_str_ptr, NODE_RIGHT_CHILD->left_child, FUNC_SCANF) == BACK_OK)
            {
                Node* args = NODE_RIGHT_CHILD->left_child;

                while(args != nullptr)
                {
                    fprintf(asm_file_ptr, "\nINP bx\n");
                    fprintf(asm_file_ptr, "PUSH bx\n");

                    for(size_t i = 0; i < VAR_NUM; i++)
                    {                   
                        if(!strcmp(VARS_ARR[i].var_text, args->left_child->left_child->value.text))
                        {
                            fprintf(asm_file_ptr, "POP [%ld]\n", i);
                            break;
                        }
                    }

                    args = args->right_child;
                }
                
                return BACK_OK;
            }

            BACK_ERROR = ERR_BCK_INVAL_ARGS_SCANF;
            ERROR_MESSAGE(stderr, ERR_BCK_INVAL_ARGS_SCANF)
            return ERR_BCK_INVAL_ARGS_SCANF;
        }
        else if(!(strcmp(NODE_LEFT_CHILD->value.text, PRINTF_LANG_DEF)))
        {
            Node* args = NODE_RIGHT_CHILD->left_child;
            size_t num_of_args = count_func_args(args);

            if(check_func_args(backend_str_ptr, args, FUNC_PRINTF) == BACK_OK)
            {
                while(args != nullptr)
                {
                    print_sub_eq(backend_str_ptr, args->left_child, asm_file_ptr, func_name);
                    fprintf(asm_file_ptr, "POP dx\n");
                    fprintf(asm_file_ptr, "PRT dx\n");

                    args = args->right_child;
                }

                return BACK_OK;
            }

            BACK_ERROR = ERR_BCK_INVAL_ARGS_PRINTF;
            ERROR_MESSAGE(stderr, ERR_BCK_INVAL_ARGS_PRINTF)
            return ERR_BCK_INVAL_ARGS_PRINTF;
        }
        else if(!(strcmp(NODE_LEFT_CHILD->value.text, SQRT_LANG_DEF)))
        {
            Node* args = NODE_RIGHT_CHILD->left_child;
            size_t num_of_args = count_func_args(args);

            if(num_of_args != 1)
            {
                BACK_ERROR = ERR_BCK_INVAL_ARGS_SQRT;
                ERROR_MESSAGE(stderr, ERR_BCK_INVAL_ARGS_SQRT)
                return ERR_BCK_INVAL_ARGS_SQRT;
            }

            if(check_func_args(backend_str_ptr, args, FUNC_PRINTF) == BACK_OK)
            {

                print_sub_eq(backend_str_ptr, args->left_child, asm_file_ptr, func_name);
                fprintf(asm_file_ptr, "SQRT\n");

                return BACK_OK;
            }

            BACK_ERROR = ERR_BCK_INVAL_ARGS_PRINTF;
            ERROR_MESSAGE(stderr, ERR_BCK_INVAL_ARGS_PRINTF)
            return ERR_BCK_INVAL_ARGS_PRINTF;
        }
    }
}

int count_func_args(Node* node_ptr) // CHECKED
{
    size_t num_of_args = 0;
    while(node_ptr != nullptr)
    {
        num_of_args++;
        node_ptr = node_ptr->right_child;
    }
    return num_of_args;
}

int check_func_args(Backend_struct* backend_str_ptr, Node* node_ptr, int flag)
{
    Node* args = node_ptr;

    if(flag == FUNC_SCANF)
    {
        while(args != nullptr)
        {
            if(NODE_LEFT_CHILD->type == VAR_HEAD)
            {
                for(size_t i = 0; i < VAR_NUM; i++)
                {
                    if(VARS_ARR[i].var_ram_id == -1)
                    {
                        BACK_ERROR = ERR_BCK_INVAL_ARGS_SCANF;
                        ERROR_MESSAGE(stderr, ERR_BCK_INVAL_ARGS_SCANF)
                        return ERR_BCK_INVAL_ARGS_SCANF;
                    }
                
                    if(!strcmp(VARS_ARR[i].var_text, NODE_LEFT_CHILD->left_child->value.text))
                    {
                        args = args->right_child;
                        break;
                    }
                }
            }
            else
            {
                BACK_ERROR = ERR_BCK_INVAL_ARGS_SCANF;
                ERROR_MESSAGE(stderr, ERR_BCK_INVAL_ARGS_SCANF)
                return ERR_BCK_INVAL_ARGS_SCANF;
            }
        }

        return BACK_OK;
    }
    else if(flag == FUNC_PRINTF)
    {
        while(args != nullptr)
        {
            if(NODE_LEFT_CHILD->type == VAL_HEAD)
            {
                args = args->right_child;
                continue;
            }
            else if(NODE_LEFT_CHILD->type == OP_HEAD)
            {
                args = args->right_child;
                continue;
            }
            else if(NODE_LEFT_CHILD->type == OP_HEAD)
            {
                args = args->right_child;
                continue;
            }
            else if(NODE_LEFT_CHILD->type == VAR_HEAD)
            {
                for(size_t i = 0; i < VAR_NUM; i++)
                {
                    if(VARS_ARR[i].var_ram_id == -1)
                    {
                        BACK_ERROR = ERR_BCK_INVAL_ARGS_PRINTF;
                        ERROR_MESSAGE(stderr, ERR_BCK_INVAL_ARGS_PRINTF)
                        return ERR_BCK_INVAL_ARGS_PRINTF;
                    }
                
                    if(!strcmp(VARS_ARR[i].var_text, NODE_LEFT_CHILD->left_child->value.text))
                    {

                        args = args->right_child;
                        break;
                    }
                }
                continue;
            }
            else if(NODE_LEFT_CHILD->type == FUNC_CALL)
            {
                args = args->right_child;
                continue;
            }
            else
            {
                BACK_ERROR = ERR_BCK_INVAL_ARGS_PRINTF;
                ERROR_MESSAGE(stderr, ERR_BCK_INVAL_ARGS_PRINTF)
                return ERR_BCK_INVAL_ARGS_PRINTF;
            }
        }
        return BACK_OK;
    }

    BACK_ERROR = ERR_BCK_INVAL_ARGS_PRINTF;
    ERROR_MESSAGE(stderr, ERR_BCK_INVAL_ARGS_PRINTF)
    return ERR_BCK_INVAL_ARGS_PRINTF;
}

int check_is_positive(double value) // CHECKED
{
    if((fabs(value - fabs(value)) < EPS) && (fabs(value) > EPS))
    {
        return IS_POSITIVE;
    }
    else if(fabs(value) <= EPS)
    {
        return IS_ZERO;
    }
    
    return IS_NEGATIVE;
}

