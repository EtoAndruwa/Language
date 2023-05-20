#include "DSL.h"

DEF_OP(Add, (int)'+', '+')  
DEF_OP(Sub, (int)'-', '-') 
DEF_OP(Mul, (int)'*', '*') 
DEF_OP(Div, (int)'/', '/') 
DEF_OP(Pow, (int)'^', '^')
DEF_OP(Eq, (int)'=', '=')


DEF_FUNC(Cos,  1, "cos") 
DEF_FUNC(Sin,  2, "sin") 
DEF_FUNC(Tan,  3, "tan") 
DEF_FUNC(Asin, 4, "asin") 
DEF_FUNC(Acos, 5, "acos") 
DEF_FUNC(Sqrt, 6, "sqrt") 
DEF_FUNC(Exp,  7, "exp") 
DEF_FUNC(Log,  8, "log") 
DEF_FUNC(Cot,  9, "cot")
DEF_FUNC(Scanf,  24, SCANF_LANG_DEF)
DEF_FUNC(Printf,  25, PRINTF_LANG_DEF)    

DEF_LOGIC(If, 10, IF_LANG_DEF)
DEF_LOGIC(Else, 11, ELSE_LANG_DEF)
DEF_LOGIC(For, 13, FOR_LANG_DEF)
DEF_LOGIC(While, 14, WHILE_LANG_DEF)
DEF_LOGIC(Break, 33, BREAK_LANG_DEF)

DEF_LOGIC_OP(Equal_logic, 70, "==")
DEF_LOGIC_OP(N_equal_logic, 71, "!=")
DEF_LOGIC_OP(Greater_logic, 72, ">")
DEF_LOGIC_OP(Greater_eq_logic, 73, ">=")


DEF_BRACK(Brack_l, (int)'(', '(')
DEF_BRACK(Brack_r, (int)')', ')')
DEF_BRACK(Fig_brack_l, (int)'{', '{')
DEF_BRACK(Fig_brack_r, (int)'}', '}')

DEF_KEY_W(Return, 21, RETURN_LANG_DEF)
DEF_KEY_W(End_line, (int)'$', '$')
DEF_KEY_W(Comma, (int)',', ',')
DEF_KEY_W(Var, 26, VAR_LANG_DEF)
DEF_KEY_W(Decl, 32, DECL_LANG_DEF)
DEF_KEY_W(Word, 27, "")
DEF_KEY_W(Val, 28, "")
DEF_KEY_W(Empty, 0, "")
DEF_KEY_W(New_line_n, 29, '\n')
DEF_KEY_W(New_line_r, 30, '\r')
DEF_KEY_W(Main, 31, MAIN_LANG_DEF)






