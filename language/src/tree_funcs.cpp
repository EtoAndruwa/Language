#include "language.h"

int ctor_tree(Tree* tree_str_ptr) // CHECKED
{
    if(tree_str_ptr == nullptr)
    {
        tree_str_ptr->error_code = ERR_TREE_NULL_STR_PTR;
        ERROR_MESSAGE(stderr, ERR_TREE_NULL_STR_PTR);
        return ERR_TREE_NULL_STR_PTR;
    }

    tree_str_ptr->error_code  = TREE_OK;     
    tree_str_ptr->num_of_toks = 0;        
    tree_str_ptr->cur_tok     = 0;            
    tree_str_ptr->root        = nullptr;

    return TREE_OK;
}

void dtor_tree(Tree* tree_str_ptr) // CHECKED
{
    dtor_childs(tree_str_ptr->root);

    tree_str_ptr->num_of_toks = TREE_POISON;
    tree_str_ptr->cur_tok     = TREE_POISON;
    tree_str_ptr->error_code  = TREE_POISON;
}

void dtor_childs(Node* node_ptr) // CHECKED
{
    if(node_ptr->left_child != nullptr)
    {
        dtor_childs(node_ptr->left_child);
    }
    if(node_ptr->right_child != nullptr)
    {
        dtor_childs(node_ptr->right_child);
    }

    node_ptr->value.node_value = TREE_POISON;
    node_ptr->type = TREE_POISON;

    free(node_ptr);
    node_ptr = nullptr;
}

Node* create_node(Tree* tree_str_ptr, double node_value, int node_type, char* text, Node* left_child, Node* right_child) // ok
{
    if((node_type == VAL || node_type == VAR || node_type == Empty) && (left_child != nullptr || right_child != nullptr))
    {
        tree_str_ptr->error_code = ERR_VAL_VAR_EMPT_HAS_CHILD;
        ERROR_MESSAGE(stderr, ERR_VAL_VAR_EMPT_HAS_CHILD)
        return nullptr;
    }

    Node* new_node_ptr = (Node*)calloc(1, sizeof(Node));

    if(new_node_ptr == nullptr)
    {   
        tree_str_ptr->error_code = ERR_TREE_TO_CALLOC_NODE;
        ERROR_MESSAGE(stderr, ERR_TREE_TO_CALLOC_NODE)
        return nullptr;
    }

    switch(node_type)
    {
        case OP_HEAD:
            new_node_ptr->type = OP_HEAD;
            new_node_ptr->left_child  = left_child;
            new_node_ptr->right_child = right_child;
            return new_node_ptr;
        case VAL_HEAD:
            new_node_ptr->type = VAL_HEAD;
            new_node_ptr->left_child  = left_child;
            new_node_ptr->right_child = right_child;
            return new_node_ptr;
        case VAR_HEAD:
            new_node_ptr->type = VAR_HEAD;
            new_node_ptr->left_child  = left_child;
            new_node_ptr->right_child = right_child;
            return new_node_ptr;
        case DECL_VAR_HEAD:
            new_node_ptr->type = DECL_VAR_HEAD;
            new_node_ptr->left_child  = left_child;
            new_node_ptr->right_child = right_child;
            return new_node_ptr;
        case DECL_FUNC_HEAD:
            new_node_ptr->type = DECL_FUNC_HEAD;
            new_node_ptr->left_child  = left_child;
            new_node_ptr->right_child = right_child;
            return new_node_ptr;
        case FUNC_ARGS:
            new_node_ptr->type = FUNC_ARGS;
            new_node_ptr->left_child  = left_child;
            new_node_ptr->right_child = right_child;
            return new_node_ptr;
        case FUNC_NAME:
            new_node_ptr->type = FUNC_NAME;
            strcpy(new_node_ptr->value.text, text);
            new_node_ptr->left_child  = left_child;
            new_node_ptr->right_child = right_child;
            return new_node_ptr;
        case FUNC_CALL:
            new_node_ptr->type = FUNC_CALL;
            new_node_ptr->left_child  = left_child;
            new_node_ptr->right_child = right_child;
            return new_node_ptr;
        case EXPR_HEAD:
            new_node_ptr->type = EXPR_HEAD;
            new_node_ptr->left_child  = left_child;
            new_node_ptr->right_child = right_child;
            return new_node_ptr;
        case LOGIC_OP_HEAD:
            new_node_ptr->type = LOGIC_OP_HEAD;
            new_node_ptr->left_child  = left_child;
            new_node_ptr->right_child = right_child;
            return new_node_ptr;
        case MAIN:
            new_node_ptr->type = MAIN;
            new_node_ptr->left_child  = left_child;
            new_node_ptr->right_child = right_child;
            return new_node_ptr;
        case FUNC_INFO:
            new_node_ptr->type = FUNC_INFO;
            new_node_ptr->left_child  = left_child;
            new_node_ptr->right_child = right_child;
            return new_node_ptr;
        case EMPTY:
            new_node_ptr->type = EMPTY;
            new_node_ptr->left_child  = nullptr;
            new_node_ptr->right_child = nullptr;
            return new_node_ptr;
        default:
            break;
    }
   
    if(node_type == VAL)
    {
        new_node_ptr->value.node_value = node_value;
        new_node_ptr->type = node_type;
        new_node_ptr->left_child  = left_child;
        new_node_ptr->right_child = right_child;
    }
    else if((node_type == VAR) && text != nullptr)
    {
        strcpy(new_node_ptr->value.text, text);
        new_node_ptr->type = node_type;
        new_node_ptr->left_child  = left_child;
        new_node_ptr->right_child = right_child; 
    }
    else if(node_type == OP)
    {
        new_node_ptr->value.op_number = int(node_value);
        new_node_ptr->type = node_type;
        new_node_ptr->left_child  = left_child;
        new_node_ptr->right_child = right_child; 
    }
    else if(node_type == LOGIC_OP)
    {
        new_node_ptr->value.op_number = int(node_value);
        new_node_ptr->type = node_type;
        new_node_ptr->left_child  = left_child;
        new_node_ptr->right_child = right_child; 
    }

    return new_node_ptr;
}
