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
DEF_FUNC(Scanf,  24, "scanf")
DEF_FUNC(Printf,  25, "printf")    

DEF_LOGIC(If, 10, "if")
DEF_LOGIC(Else, 11, "else")
DEF_LOGIC(Else_if, 12, "else_if")
DEF_LOGIC(For, 13, "for")
DEF_LOGIC(While, 14, "while")

DEF_BRACK(Brack_l, (int)'(', '(')
DEF_BRACK(Brack_r, (int)')', ')')
DEF_BRACK(Sqr_brack_l, (int)'[', '[')
DEF_BRACK(Sqr_brack_r, (int)']', ']')
DEF_BRACK(Fig_brack_l, (int)'{', '{')
DEF_BRACK(Fig_brack_r, (int)'}', '}')

DEF_KEY_W(Return, 21, "return")
DEF_KEY_W(End_line, (int)'$', '$')
DEF_KEY_W(Comma, (int)',', ',')
DEF_KEY_W(Var, 26, "var")
DEF_KEY_W(Decl, 32, "decl")
DEF_KEY_W(Word, 27, "")
DEF_KEY_W(Val, 28, "")
DEF_KEY_W(Empty, 0, "")
DEF_KEY_W(New_line_n, 29, '\n')
DEF_KEY_W(New_line_r, 30, '\r')
DEF_KEY_W(Main, 31, "main")






