#include "main.h"

#define OP_CONDITION(x) (x.type == TT_NUMBER || x.type == TT_VARIABLE || x.name == CBRACK)

int lex(char input_string[], token formula[])
{
    int i, f, f_index = 0, counter = 0, counter2 = 0;
    char temp[20], var;
    token previous_token;
    previous_token = keywords[T_NULL];

    if (input_string[0] != 'f' || input_string[1] != '(' || input_string[3] != ')' || input_string[4] != '=')
        return err(PARSER, "your function should start like: \"f(x) = ...\"");

    var = input_string[2];
    counter = 0;

    if (var == 'e' || var == '+' || var == '-' || var == '*' || var == '/' || var == '^' || var == '%' || var == '(' || var == ')' || var == '|' || var == '.' || isdigit(var))
        return err(PARSER, "\"+\", \"-\", \"*\", \"/\", \"^\", \"%%\", \"(\", \")\", \"|\", \"e\", \".\" and 0-9 digits cannot be used as variable name.");

    for (i = 5; input_string[i] != '\0'; i++)
    {
        token current_token = keywords[T_NULL];

        if (isdigit(input_string[i]) && previous_token.type != TT_VARIABLE) {
            int x = i + 1, t = 1;
            temp[0] = input_string[i];
            for (++i; isdigit(input_string[i]) || input_string[i] == '.'; i++)
            {
                temp[t] = input_string[i];
                t++;
            }

            temp [t] = '\0'; i--;

            if (is_valid_number(temp)) {
                if (previous_token.name == CBRACK) {
                    formula[f_index] = keywords[T_MUL];
                    f_index++;
                }
                current_token.type = TT_NUMBER;
                current_token.value = atof(temp);
            } else {
                return err(PARSER, "invalid formula. (3)");
            }
        } else if (input_string[i] == '+' && OP_CONDITION(previous_token)) current_token = keywords[T_ADD];
        else if (input_string[i] == '-' && OP_CONDITION(previous_token)) current_token = keywords[T_SUB];
        else if (input_string[i] == '*' && OP_CONDITION(previous_token)) current_token = keywords[T_MUL];
        else if (input_string[i] == '/' && OP_CONDITION(previous_token)) current_token = keywords[T_DIV];
        else if (input_string[i] == '%' && OP_CONDITION(previous_token)) current_token = keywords[T_MOD];
        else if (input_string[i] == '^' && OP_CONDITION(previous_token)) current_token = keywords[T_POW];
        else if (input_string[i] == ')' && OP_CONDITION(previous_token)) current_token = keywords[T_CBRACK];
        else if (input_string[i] == '-' && (previous_token.name == OBRACK || previous_token.type == TT_OPERATOR || previous_token.type == TT_NULL)) {
            current_token = keywords[T_NEG];
        } else if (isalpha(input_string[i]) || input_string[i] == var) {

            if (!isalpha(var)) current_token = keywords[T_VAR_X];
            int type = determine_type(input_string, i, var);

            if (type > 0)
                current_token = keywords[type];
            else
                return err(PARSER, "invalid formula. (4)");

            if ((current_token.type == TT_VARIABLE || current_token.type == TT_FUNCTION) && OP_CONDITION(previous_token)) {
                formula[f_index] = keywords[T_MUL];
                f_index++;
            }

            switch (type)
            {
                case T_SIN: case T_COS: case T_TAN: case T_COT: case T_SEC: case T_CSC: case T_LOG:
                i += 2; break;

                case T_VAR_PI: case T_LN:
                i++; break;

                case T_SQRT: i += 3; break;
            }
        } else if (input_string[i] == '|') {
            if (previous_token.type == TT_OPERATOR || previous_token.name == FUNC_NEG || previous_token.name == OBRACK || previous_token.type == TT_NULL)
                current_token = keywords[T_ABS];
            else if (OP_CONDITION(previous_token))
                current_token = keywords[T_CBRACK];
            else
                return err(PARSER, "invalid absolute function notation (|x|).");
        } else if (input_string[i] == '(') {
            current_token = keywords[T_OBRACK];

            if (OP_CONDITION(previous_token)) {
                formula[f_index] = keywords[T_MUL];
                f_index++;
            }
        } else if (input_string[i] == ';' && OP_CONDITION(previous_token) && input_string[i + 1] == '\0') {
            current_token = keywords[T_TERMINAL];
        } else {
            return err(PARSER, "invalid formula. (5)");
        }

        previous_token = keywords[T_NULL];
        previous_token = current_token;
        formula[f_index] = current_token;
        f_index++;

        if (current_token.name == FUNC_ABS)
        {
            formula[f_index] = keywords[T_OBRACK];
            previous_token = keywords[T_OBRACK];
            f_index++;
        }
    }

    for (i = 0; formula[i].type != TT_TERMINAL; i++)
    {
        if (formula[i].name == OBRACK) counter++;
        if (formula[i].name == CBRACK) counter2++;
    }

    if (counter != counter2) return err(PARSER, "mismatched brackets.");

    return VALID;
}

int is_valid_number(char *s)
{
    int i, dot_points;
    for (i = 1; s[i] == '\0'; i++) if (s[i] == '.') dot_points++;

    if (dot_points > 1)
        return -1;
    
    return 1;
}

int determine_type(char input[], int index, int xvar)
{
    int i;
    char s3[6], s4[6], s5[6];

    for (i = 0; i < 3; i++) s3[i] = input[index + i]; s3[i] = '\0';
    for (i = 0; i < 4; i++) s4[i] = input[index + i]; s4[i] = '\0';
    for (i = 0; i < 5; i++) s5[i] = input[index + i]; s5[i] = '\0';

    if (strcmp(s4, "sin(") == 0) return T_SIN;
    else if (strcmp(s4, "sec(") == 0) return T_SEC;
    else if (strcmp(s5, "sqrt(") == 0) return T_SQRT;
    else if (strcmp(s4, "cos(") == 0) return T_COS;
    else if (strcmp(s4, "cot(") == 0) return T_COT;
    else if (strcmp(s4, "csc(") == 0) return T_CSC;
    else if (strcmp(s4, "tan(") == 0) return T_TAN;
    else if (strcmp(s3, "ln(") == 0) return T_LN;
    else if (strcmp(s4, "log(") == 0) return T_LOG;
    else if (input[index] == 'p') {
        if (input[index + 1] == 'i')
            return T_VAR_PI;
    } else if (input[index] == 'e') return T_VAR_E;
    else if (input[index] == xvar) return T_VAR_X;

    return 0;
}

    // if (input[index] == 's') {
    //     if (strcmp(s4, "sin(") == 0) return T_SIN;
    //     else if (strcmp(s4, "sec(") == 0) return T_SEC;
    //     else if (strcmp(s5, "sqrt(") == 0) return T_SQRT;
    //     else if (xvar == 's') return T_VAR_X;
    // } else if (input[index] == 'c') {
    //     if (strcmp(s4, "cos(") == 0) return T_COS;
    //     else if (strcmp(s4, "cot(") == 0) return T_COT;
    //     else if (strcmp(s4, "csc(") == 0) return T_CSC;
    //     else if (xvar == 'c') return T_VAR_X;
    // } else if (input[index] == 't') {
    //     if (strcmp(s4, "tan(") == 0) return T_TAN;
    //     else if (xvar == 't') return T_VAR_X;
    // } else if (input[index] == 'l') {
    //     if (strcmp(s3, "ln(") == 0) return T_LN;
    //     else if (strcmp(s4, "log(") == 0) return T_LOG;
    //     else if (xvar == 'l') return T_VAR_X;
    // } else if (input[index] == 'p') {
    //     if (input[index + 1] == 'i') return T_VAR_PI;
    //     else if (xvar == 'p') return T_VAR_X;
    // } else if (input[index] == 'e') {
    //     return T_VAR_E;
    // } else if (input[index] == xvar) {
    //     return T_VAR_X;
    // }