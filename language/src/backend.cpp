#include "language.h"

int ctor_backend(Backend_struct* backend_str_ptr) // CHECKED
{
    VAR_NUM     = 1;
    FUNC_NUM    = 1;
    CUR_RAM_ID  = 0;
    CUR_FUNC_ID = 0;
    CUR_VAR_ID  = 0;
    CUR_DECL_ID = 0;
    DECL_NUM    = 1;
    backend_str_ptr->main_node_id = 0;

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

    backend_str_ptr->error_code   = LEX_POISON;
    backend_str_ptr->main_node_id = LEX_POISON;

    return BACK_OK;
}

void print_vars(Backend_struct* backend_str_ptr) // CHECKED
{
    printf("\n--------------------VARS--------------------\n");
    for(size_t i = 0; i < VAR_NUM; i++)
    {
        printf("VARS_ARR[%ld].var_text = %s, VARS_ARR[%ld].var_ram_id = %ld\n", i, VARS_ARR[i].var_text, i, VARS_ARR[i].var_ram_id);
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

    if(translate_expr(backend_str_ptr, DECL_NODES[backend_str_ptr->main_node_id]->left_child, asm_file_ptr) != BACK_OK)
    {
        ERROR_MESSAGE(stderr, ERR_BCK_TRANSLATE_MAIN)
        return ERR_BCK_TRANSLATE_MAIN;
    }
    fprintf(asm_file_ptr, "HTL\n");


    for(size_t i = 0; i < DECL_NUM; i++)
    {
        if(DECL_NODES[i] != nullptr)
        {
            if(backend_str_ptr->main_node_id == i)
            {
                continue;
            }

            if(print_decl_funcs(backend_str_ptr, DECL_NODES[i], asm_file_ptr) != BACK_OK)
            {
                ERROR_MESSAGE(stderr, ERR_BCK_TRANSLATE_FUNC_DECL)
                BACK_ERROR = ERR_BCK_TRANSLATE_FUNC_DECL;
                return ERR_BCK_TRANSLATE_FUNC_DECL;
            }
            return BACK_OK;  
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

int translate_expr(Backend_struct* backend_str_ptr, Node* node_ptr, FILE* asm_file_ptr) // CHECKED
{
    if(node_ptr == nullptr)
    {
        return BACK_OK;
    }

    if(node_ptr->type == EXPR_HEAD)
    {
        if(NODE_LEFT_CHILD->type == OP_HEAD)
        {
            if(translate_var_decl(backend_str_ptr, NODE_LEFT_CHILD->left_child, asm_file_ptr) != BACK_OK)
            {
                ERROR_MESSAGE(stderr, ERR_BCK_TRANSLATE_VAR_DECL)
                return ERR_BCK_TRANSLATE_VAR_DECL;
            }
        }
        else if(NODE_LEFT_CHILD->type == RETURN)
        {
            if(print_sub_eq(backend_str_ptr, NODE_LEFT_CHILD->left_child->left_child, asm_file_ptr) != BACK_OK)
            {
                ERROR_MESSAGE(stderr, ERR_BCK_TRANSLATE_SUB_EQ)
                return ERR_BCK_TRANSLATE_SUB_EQ;
            }
            fprintf(asm_file_ptr, "POP ax\n");
        }   
        
        if(translate_expr(backend_str_ptr, NODE_RIGHT_CHILD, asm_file_ptr) != BACK_OK)
        {
            ERROR_MESSAGE(stderr, ERR_BCK_TRANSLATE_EXPR)
            return ERR_BCK_TRANSLATE_EXPR;
        }
        return BACK_OK;
    }

    ERROR_MESSAGE(stderr, ERR_BCK_NEW_TYPE_EXPR)
    BACK_ERROR = ERR_BCK_NEW_TYPE_EXPR;
    return ERR_BCK_NEW_TYPE_EXPR;
}

int translate_var_decl(Backend_struct* backend_str_ptr, Node* node_ptr, FILE* asm_file_ptr) // CHECKED
{
    if(realloc_vars(backend_str_ptr) != BACK_OK)
    {
        ERROR_MESSAGE(stderr, ERR_BCK_REALLOC_VARS);
        return ERR_BCK_REALLOC_VARS;
    }

    if(NODE_LEFT_CHILD->type == DECL_VAR_HEAD)
    {
        if(print_sub_eq(backend_str_ptr, NODE_RIGHT_CHILD, asm_file_ptr) != BACK_OK)
        {
            ERROR_MESSAGE(stderr, BACK_ERROR)
            BACK_ERROR = DECL_VAR_HEAD;
            return BACK_ERROR;
        }
        fprintf(asm_file_ptr, "POP [%ld]\n\n", CUR_RAM_ID);

        strcpy(VARS_ARR[CUR_VAR_ID].var_text, NODE_LEFT_CHILD->left_child->left_child->value.text);
        VARS_ARR[CUR_VAR_ID].var_ram_id = CUR_RAM_ID;


        CUR_RAM_ID++; // empty index for the new var in the CPU's RAM
        CUR_VAR_ID++; // empty index in the inner var's array of backend
        return BACK_OK;
    }
    else if(NODE_LEFT_CHILD->type == VAR_HEAD)
    {
        if(translate_var_assign(backend_str_ptr, node_ptr, asm_file_ptr) != BACK_OK)
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

int print_sub_eq(Backend_struct* backend_str_ptr, Node* node_ptr, FILE* asm_file_ptr) // CHECKED
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
        
            if(!strcmp(VARS_ARR[i].var_text, NODE_LEFT_CHILD->value.text))
            {
                fprintf(asm_file_ptr, "PUSH [%ld]\n", i);
                return BACK_OK;
            }
        }
        return BACK_OK; 
    }   
    else if(node_ptr->type == OP_HEAD)
    {
        if(print_sub_eq(backend_str_ptr, NODE_LEFT_CHILD, asm_file_ptr) != BACK_OK)
        {
            ERROR_MESSAGE(stderr, BACK_ERROR)
            return BACK_ERROR;
        }
        return BACK_OK;
    }
    else if(node_ptr->type == FUNC_CALL)
    {
        printf("CALL\n");
        if(print_call_func(backend_str_ptr, NODE_LEFT_CHILD, asm_file_ptr) != BACK_OK)
        {
            ERROR_MESSAGE(stderr, BACK_ERROR)
            return BACK_ERROR;
        }
        return BACK_OK;
    }

    if(print_sub_eq(backend_str_ptr, NODE_LEFT_CHILD, asm_file_ptr) != BACK_OK)
    {
        ERROR_MESSAGE(stderr, BACK_ERROR)
        return BACK_ERROR;   
    }
    if(print_sub_eq(backend_str_ptr, NODE_RIGHT_CHILD, asm_file_ptr) != BACK_OK)
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

int translate_var_assign(Backend_struct* backend_str_ptr, Node* node_ptr, FILE* asm_file_ptr) // CHECKED
{
    int var_id = -1;  // flag for found_var_id

    for(size_t i = 0; i < VAR_NUM; i++)
    {
        if(VARS_ARR[i].var_ram_id == -1)
        {
            break;
        }
        if(!strcmp(VARS_ARR[i].var_text, NODE_LEFT_CHILD->left_child->value.text))
        {
            var_id = i;  
            break;
        }
    }

    if(var_id != -1)
    {
        if(print_sub_eq(backend_str_ptr, NODE_RIGHT_CHILD, asm_file_ptr) != BACK_OK)
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

int print_decl_funcs(Backend_struct* backend_str_ptr, Node* node_ptr, FILE* asm_file_ptr) // CHECKED
{
    if(node_ptr->type == DECL_FUNC_HEAD)
    {
        fprintf(asm_file_ptr, "\n\n%s:\n", NODE_LEFT_CHILD->left_child->value.text);
        translate_expr(backend_str_ptr, NODE_LEFT_CHILD->right_child->right_child, asm_file_ptr);
        fprintf(asm_file_ptr, "RET\n");

        return BACK_OK;
    }

    BACK_ERROR = ERR_BCK_NEW_TYPE_DECL_FUNC;
    ERROR_MESSAGE(stderr, ERR_BCK_NEW_TYPE_DECL_FUNC)
    return ERR_BCK_NEW_TYPE_DECL_FUNC;
}

int print_call_func(Backend_struct* backend_str_ptr, Node* node_ptr, FILE* asm_file_ptr) // CHECKED
{
    if(node_ptr->type == FUNC_HEAD)
    {
        Node* func_args_node = NODE_RIGHT_CHILD->left_child;

        while(func_args_node != nullptr)
        {
            print_sub_eq(backend_str_ptr, func_args_node->left_child, asm_file_ptr);
            func_args_node = func_args_node->right_child;
        }

        fprintf(asm_file_ptr, "CALL %s:\n", NODE_LEFT_CHILD->value.text);
        fprintf(asm_file_ptr, "PUSH ax\n");

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