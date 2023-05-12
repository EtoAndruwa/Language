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
    if((node_type == VAL || node_type == VAR || node_type == EMPTY || node_type == ERROR) && (left_child != nullptr || right_child != nullptr))
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

    if(node_type == FUNC_NAME)
    {
        new_node_ptr->type = FUNC_NAME;
        strcpy(new_node_ptr->value.text, text);
        new_node_ptr->left_child  = left_child;
        new_node_ptr->right_child = right_child;
        return new_node_ptr;
    }
    else if(node_type == EMPTY || node_type == ERROR)
    {
        new_node_ptr->type = node_type;
        new_node_ptr->value.op_number = node_type;
        new_node_ptr->left_child  = nullptr;
        new_node_ptr->right_child = nullptr;
        return new_node_ptr;
    }
    else if(node_type == VAL)
    {
        new_node_ptr->value.node_value = node_value;
        new_node_ptr->type = node_type;
        new_node_ptr->left_child  = left_child;
        new_node_ptr->right_child = right_child;
        return new_node_ptr;
    }
    else if((node_type == VAR) && text != nullptr)
    {
        printf("VAR TEXT: %s\n",text);
        strcpy(new_node_ptr->value.text, text);
        new_node_ptr->type = node_type;
        new_node_ptr->left_child  = left_child;
        new_node_ptr->right_child = right_child; 
        return new_node_ptr;
    }

    new_node_ptr->value.op_number = int(node_value);
    new_node_ptr->type = node_type;
    new_node_ptr->left_child  = left_child;
    new_node_ptr->right_child = right_child; 
    
    return new_node_ptr;
}
