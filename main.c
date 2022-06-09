#include "main.h"
#define SDL_MAIN_HANDLED

token final_formula[200];

int main(int argc, char *argv[])
{
    token formula[200];
    double value;
    bool valid_formula = false;

    while (!valid_formula)
    {
        char input[200];
        int c, i = 0;

        printf("Enter a function: ");

        while ((c = getchar()) != '\n')
        {
            if (isalpha(c)) c = tolower(c);
            if (isspace(c)) continue;
            input[i] = c;
            i++;
        }

        input[i] = '\0';

        if (lex(input, formula)) {
            to_postfix_notation(formula);
            if (is_mathematically_valid()) {
                valid_formula = true;
            }
        }
    }

    token result;
    printf("Enter a value for %c: ", var);
    scanf("%lf", &value);

    result = calculate_for(value, true);
    if (result.type != TT_NULL) printf("Result: %lf\n", result.value);

    plot(); 

    return 0;
}

void push(token item, tstack *stack)
{
    stack->top++;
    stack->s[stack->top] = item;
}

token pop(tstack *stack)
{
    token temp = stack->s[stack->top];
    stack->top--;
    return temp;
}

bool err(int type, char *message)
{
    switch(type)
    {
        case PARSER: printf("Parse error: %s\n\n", message); break;
        case CALCULATOR: printf("Calculation error: %s\n\n", message); break;
        case SDL: printf("SDL error: %s\n\n", message); break;
    }

    return false;
}

// For debugging purposes
void print_expr(token expr[])
{
    int i;
    for (i = 0; expr[i].type != TT_TERMINAL; i++) {
        if (expr[i].type == TT_NUMBER) printf("%f, ", expr[i].value);
        else if (expr[i].name == VAR_E) printf("e, ");
        else if (expr[i].name == VAR_PI) printf("pi, ");
        else if (expr[i].name == VAR_X) printf("var, ");
        else if (expr[i].name == OP_ADD) printf("+, ");
        else if (expr[i].name == OP_SUB) printf("-, ");
        else if (expr[i].name == OP_MUL) printf("*, ");
        else if (expr[i].name == OP_DIV) printf("/, ");
        else if (expr[i].name == OP_MOD) printf("%%, ");
        else if (expr[i].name == OP_POW) printf("^, ");
        else if (expr[i].name == FUNC_ABS) printf("abs, ");
        else if (expr[i].name == FUNC_SQRT) printf("sqrt, ");
        else if (expr[i].name == FUNC_SIN) printf("sin, ");
        else if (expr[i].name == FUNC_COS) printf("cos, ");
        else if (expr[i].name == FUNC_TAN) printf("tan, ");
        else if (expr[i].name == FUNC_COT) printf("cot, ");
        else if (expr[i].name == FUNC_SEC) printf("sec, ");
        else if (expr[i].name == FUNC_CSC) printf("csc, ");
        else if (expr[i].name == FUNC_LN) printf("ln, ");
        else if (expr[i].name == FUNC_LOG) printf("log, ");
        else if (expr[i].name == FUNC_NEG) printf("neg, ");
        else if (expr[i].name == OBRACK) printf("(, ");
        else if (expr[i].name == CBRACK) printf("), ");
        else if (expr[i].type == TT_NULL) printf("null, ");
    }

    printf(";");
    printf("\n%d\n", i + 1);
}