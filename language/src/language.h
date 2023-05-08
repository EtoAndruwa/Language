#ifndef LANGAUGE_H
#define LANGAUGE_H

/*####################################################################################################################################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

/*####################################################################################################################################################################*/

#define EPS 1e-7        // Used in comparisson of doubles
#define PI  3.14159265  // Used in calculations
const int TREE_POISON = 0xDEAD;

/*####################################################################################################################################################################*/
                    
#include "../../graphviz/src/graphviz.h"  // The graphviz lib's header with funcs
#include "DSL.h"
#include "lexer.h"
#include "tree.h"

/*####################################################################################################################################################################*/

enum return_codes
{

};


enum error_codes_front
{
    FRONT_OK               = 0,
    ERR_FRT_NO_MAIN        = -1,
    ERR_FRT_MAIN_BRACK     = -2,
    ERR_FRT_NO_OPEN_SQR_BR = -3,
    ERR_FRT_NO_CLOS_SQR_BR = -4,
    ERR_FRT_NO_OPEN_BR     = -5,
    ERR_FRT_NO_CLOS_BR     = -6,
    ERR_FRT_NO_OPEN_FIG_BR = -7,
    ERR_FRT_NO_CLOS_FIG_BR = -8,
    ERR_FRT_NO_END_LINE    = -9,
};

enum error_codes_back
{

};

Node* get_recur_tree(Tree* tree_ptr, token* tok_arr_ptr);

#endif
