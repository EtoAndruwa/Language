#include "language.h"

int ctor_backend(Backend_struct* backend_str_ptr)
{
    VAR_NUM     = 10;
    FUNC_NUM    = 10;
    RAM_CUR_ID  = 0;
    CUR_FUNC_ID = 0;
    CUR_VAR_ID  = 0;
    CUR_DECL_ID = 0;
    DECL_NUM    = 10;
    backend_str_ptr->main_node_id = 0;

    backend_str_ptr->funcs = (func_info*)calloc(FUNC_NUM, sizeof(func_info));
    if(backend_str_ptr->funcs == nullptr)
    {
        BACK_ERROR = ERR_BCK_CALLOC_FUNCS;
        ERROR_MESSAGE(stderr, ERR_BCK_CALLOC_FUNCS)
        return ERR_BCK_CALLOC_FUNCS;
    }

    printf("%ld\n", FUNC_NUM);
    for(size_t i = 0; i < VAR_NUM; i++)
    {
        strcpy(FUNCS_ARR[i].func_name, "EMPTY");
        FUNCS_ARR[i].num_of_vars = 0;
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
        VARS_ARR[i].var_ram_id = 0;
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

int dtor_backend(Backend_struct* backend_str_ptr)
{
    backend_str_ptr->cur_ram_id = LEX_POISON;

    free(backend_str_ptr->funcs);
    backend_str_ptr->funcs = nullptr;

    free(backend_str_ptr->vars);
    backend_str_ptr->vars = nullptr;

    free(backend_str_ptr->decl_nodes);
    backend_str_ptr->decl_nodes = nullptr;

    backend_str_ptr->num_of_funcs = LEX_POISON;
    backend_str_ptr->num_of_vars = LEX_POISON;

    return BACK_OK;
}

void print_vars(Backend_struct* backend_str_ptr)
{
    printf("\n--------------------VARS--------------------\n");
    for(size_t i = 0; i < VAR_NUM; i++)
    {
        printf("VARS_ARR[%ld].var_text = %s, VARS_ARR[%ld].var_ram_id = %ld\n", i, VARS_ARR[i].var_text, i, VARS_ARR[i].var_ram_id);
    }
    printf("--------------------VARS--------------------\n");
}

void print_funcs(Backend_struct* backend_str_ptr)
{
    printf("\n--------------------FUNCS--------------------\n");
    for(size_t i = 0; i < FUNC_NUM; i++)
    {
        printf("FUNC[%ld].name = %s, FUNC[%ld].num_of_vars = %ld\n", i, FUNCS_ARR[i].func_name, i, FUNCS_ARR[i].num_of_vars);
    }
    printf("--------------------FUNCS--------------------\n");
}

int create_asm(Backend_struct* backend_str_ptr)
{
    FILE* asm_file_ptr = fopen(FILE_ASM_NAME, "w");
    if(asm_file_ptr == nullptr)
    {
        ERROR_MESSAGE(stderr, ERR_BCK_OPEN_ASM_FILE)
        BACK_ERROR = ERR_BCK_OPEN_ASM_FILE;
        return ERR_BCK_OPEN_ASM_FILE;
    }

    printf("Main id = %ld\n", backend_str_ptr->main_node_id);
    translate_expr(backend_str_ptr, DECL_NODES[backend_str_ptr->main_node_id]->left_child, asm_file_ptr);

    if(fclose(asm_file_ptr) == EOF)
    {
        ERROR_MESSAGE(stderr, ERR_BCK_CLOSE_ASM_FILE)
        BACK_ERROR = ERR_BCK_CLOSE_ASM_FILE;
        return ERR_BCK_CLOSE_ASM_FILE;
    }
    return BACK_OK;
}

Node* find_main_node(Backend_struct* backend_str_ptr, Node* node_ptr)
{
    if(node_ptr == nullptr)
    {
        return nullptr;
    }

    if(node_ptr->type == EXPR_HEAD)
    {
        if(NODE_LEFT_CHILD->type == MAIN)
        {
            DECL_NODES[CUR_DECL_ID] = NODE_LEFT_CHILD;
            backend_str_ptr->main_node_id = CUR_DECL_ID;
            CUR_DECL_ID++;
        }

        if(NODE_LEFT_CHILD->type == DECL_FUNC_HEAD)
        {
            DECL_NODES[CUR_DECL_ID] = NODE_LEFT_CHILD;
            CUR_DECL_ID++;
        }
        
        return find_main_node(backend_str_ptr, NODE_RIGHT_CHILD);
    }
}

void print_decls(Backend_struct* backend_str_ptr)
{
    printf("\n--------------------DECLS--------------------\n");
    for(size_t i = 0; i < DECL_NUM; i++)
    {
        printf("backend_str_ptr->decl_nodes[%ld] = %p\n", i, backend_str_ptr->decl_nodes[i]);
    }
    printf("--------------------DECLS--------------------\n");
}

void translate_expr(Backend_struct* backend_str_ptr, Node* node_ptr, FILE* asm_file_ptr)
{
    if(node_ptr == nullptr)
    {
        return;
    }
    if(node_ptr->type == EXPR_HEAD && NODE_LEFT_CHILD->type == RETURN)
    {
        return;
    }

    if(node_ptr->type == EXPR_HEAD)
    {
        printf("HERE1\n");
        if(NODE_LEFT_CHILD->type == OP_HEAD)
        {
            printf("HERE2\n");
            translate_var_decl(backend_str_ptr, NODE_LEFT_CHILD->left_child, asm_file_ptr);
        }
        translate_expr(backend_str_ptr, NODE_RIGHT_CHILD, asm_file_ptr);
    }
}

void translate_var_decl(Backend_struct* backend_str_ptr, Node* node_ptr, FILE* asm_file_ptr)
{
    printf("HERE3\n");
    if(NODE_LEFT_CHILD->type == DECL_VAR_HEAD)
    {
        printf("\nfirst translate\n");
        if(NODE_RIGHT_CHILD->type == VAL_HEAD)
        {
            fprintf(asm_file_ptr, "PUSH %f\n", NODE_RIGHT_CHILD->left_child->value.node_value);
            fprintf(asm_file_ptr, "POP [%ld]\n", RAM_CUR_ID);
        }
        else
        {
            print_sub_eq(backend_str_ptr, NODE_RIGHT_CHILD, asm_file_ptr);
        }

        strcpy(VARS_ARR[CUR_VAR_ID].var_text, NODE_LEFT_CHILD->left_child->left_child->value.text);
        VARS_ARR[CUR_VAR_ID].var_ram_id = RAM_CUR_ID;
        RAM_CUR_ID++;
        CUR_VAR_ID++;
    }
    // else
    // {
    //     translate_var_assign(backend_str_ptr, node_ptr, asm_file_ptr);
    // }
}

void print_sub_eq(Backend_struct* backend_str_ptr, Node* node_ptr, FILE* asm_file_ptr)
{
    if(node_ptr == nullptr)
    {
        return;
    }

    if(node_ptr->type == VAL_HEAD)
    {
        fprintf(asm_file_ptr, "PUSH %f\n", NODE_LEFT_CHILD->value.node_value);
        return;
    }

    if(node_ptr->type == OP_HEAD)
    {
        print_sub_eq(backend_str_ptr, NODE_LEFT_CHILD, asm_file_ptr);
        return;
    }

    print_sub_eq(backend_str_ptr, NODE_LEFT_CHILD, asm_file_ptr);
    print_sub_eq(backend_str_ptr, NODE_RIGHT_CHILD, asm_file_ptr);
    
    switch(node_ptr->value.op_number)
    {
        case Add:
            fprintf(asm_file_ptr, "ADD\n");
            break;
        case Sub:
            fprintf(asm_file_ptr, "SUB\n");
            break;
        case Div:
            fprintf(asm_file_ptr, "DIV\n");
            break;
        case Mul:
            fprintf(asm_file_ptr, "MUL\n");
            break;
        default:
            fprintf(asm_file_ptr, "ERROR_OP\n");
            break;
    }
}

// void translate_var_assign(Backend_struct* backend_str_ptr, Node* node_ptr, FILE* asm_file_ptr)
// {


    
// }

