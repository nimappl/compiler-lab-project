#include "main.h"

#define IS_SIN_ZERO(x) ((fmod((x / M_PI), 2)) == 0)
#define IS_COS_ZERO(x) ((fmod(x, (M_PI/2)) == 0 && fmod(x, M_PI)) != 0)

//  Converts infix notation formula to postfix notation.
void to_postfix_notation(token formula[])
{
    int i, j = 0;
    tstack stack;
    stack.top = -1;

    for (i = 0; formula[i].type != TT_TERMINAL; i++)
    {
        if (formula[i].name == OBRACK) {
            push(formula[i], &stack);
        } else if (formula[i].name == CBRACK) {
            while (stack.s[stack.top].name != OBRACK)
            {
                final_formula[j] = pop(&stack);
                j++;
            }

            pop(&stack);
        } else if (formula[i].type == TT_OPERATOR || formula[i].type == TT_FUNCTION) {
            if (stack.top != -1) {
                if (formula[i].name == OP_POW && stack.s[stack.top].name == OP_POW) {
                    push(formula[i], &stack);
                } else if (formula[i].presedence > stack.s[stack.top].presedence)
                    push(formula[i], &stack);
                else {
                    //this doesn't work as expected, hence the last +1
                    while (stack.top != -1 && formula[i].presedence < stack.s[stack.top].presedence + 1)
                    {
                        final_formula[j] = pop(&stack);
                        j++;
                    }

                    push(formula[i], &stack);
                }
            } else {
                push(formula[i], &stack);
            }
        } else if (formula[i].type == TT_VARIABLE || formula[i].type == TT_NUMBER) {
            final_formula[j] = formula[i];
            j++;
        }
    }

    while (stack.top != -1)
    {
        final_formula[j] = pop(&stack);
        j++;
    }

    final_formula[j] = keywords[T_TERMINAL];
}

/*
    Tests the formula of the function for 80 numbers between -400 and 400.
    returns false if all fail, which indicates that the formula is mathematically vague,
    regardless of value given as independant variable.
*/
bool is_mathematically_valid()
{
    int counter = 0;
    token result;

    for (double i = -400; i < 400; i+=10)
    {
        result = calculate_for(i, false);
        if (result.type == TT_NULL) counter++;
        if (result.name == T_TERMINAL) {
            err(CALCULATOR, "formula is invalid (4)");
            return false;
        }
    }

    return (counter == 80) ? err(CALCULATOR, "Function is not mathematically valid.") : true;
}

/*
    Calculates the result for given value as independant variable, using shunting yard algorithm.

    return type :
    TT_NULL if the formula is mathematically vague for given value as independant variable.
    TT_NULL with name parameter of TERMINAL if stack is not empty at the end.
*/
token calculate_for(double value, bool verbose)
{
    tstack stack;
    stack.top = -1;

    for (int i = 0; final_formula[i].type != TT_TERMINAL; i++)
    {
        if (final_formula[i].type == TT_NUMBER) {
            push(final_formula[i], &stack);
        } else if (final_formula[i].type == TT_VARIABLE) {
            token t = { TT_NUMBER, NUM, 0, 0 };

            if (final_formula[i].name == VAR_X) t.value = value;
            else t.value = final_formula[i].value;

            push(t, &stack);
        } else if (final_formula[i].type == TT_OPERATOR) {
            token op, operand1, operand2, result = { TT_NUMBER, NUM, 0, 0 };
            
            op = final_formula[i];
            operand2 = pop(&stack);
            operand1 = pop(&stack);
            result = calculate(op, operand1.value, operand2.value, verbose);

            if (result.type == TT_NULL) return result;
            push(result, &stack);

        } else if (final_formula[i].type == TT_FUNCTION) {
            token func = final_formula[i], result = { TT_NUMBER, NUM, 0, 0 };
            token operand = pop(&stack);

            result = calculate(func, operand.value, 0, verbose);

            if (result.type == TT_NULL) return result;

            push(result, &stack);
        }
    }

    if (stack.top != 0) return (token){ TT_NULL, TERMINAL, 0, 0 };

    return stack.s[stack.top];
}

/*
    Returns TT_NULL if the formula is mathematically vague for given values.
    Shows errors if verbose is set to "true".
*/
token calculate(token operation, double operand1, double operand2, bool verbose)
{
    token result = { TT_NUMBER, NUM, 0, 0 }, vague = { TT_NULL, NONE, 0, 0 };

    if (fabs(operand2) < 0.000001) operand2 = 0; // resolve almost-but-not-quite-zero cases
    if (fabs(operand1) < 0.000001) operand1 = 0;

    if (operation.type == TT_OPERATOR) {
        switch (operation.name)
        {
            case OP_ADD: result.value = operand1 + operand2; break;
            case OP_SUB: result.value = operand1 - operand2; break;
            case OP_MUL: result.value = operand1 * operand2; break;
            case OP_DIV:
                if (operand2 == 0) {
                    if (verbose) err(CALCULATOR, "division by 0.");
                    result = vague;
                } else result.value = operand1 / operand2;
                break;
            case OP_MOD:
                if (operand2 == 0) {
                    if (verbose) err(CALCULATOR, "division by 0.");
                    result = vague;
                } else result.value = fmod(operand1, operand2);
                break;
            case OP_POW: 
                if ((operand1 < 0) && (operand2 != (int)operand2)) {
                    if (verbose) err(CALCULATOR, "given value is out of domain of power function.");
                    result = vague;
                } else result.value = pow(operand1, operand2); break;
        }
    } else {
        switch (operation.name)
        {
            case FUNC_SIN: result.value = sin(operand1); break;
            case FUNC_COS: result.value = cos(operand1); break;
            case FUNC_TAN:
                if (IS_COS_ZERO(operand1)) {
                    if (verbose) err(CALCULATOR, "Tangent is infinity for given value.");
                    result = vague;
                } else result.value = tan(operand1);
                break;
            case FUNC_COT:
                if (IS_SIN_ZERO(operand1)) {
                    if (verbose) err(CALCULATOR, "Cotangent is infinity for given value.");
                    result = vague;
                } else result.value = cos(operand1) / sin(operand1);
                break;
            case FUNC_SEC:
                if (IS_COS_ZERO(operand1)) {
                    if (verbose) err(CALCULATOR, "Secant is infinity for given value.");
                    result = vague;
                } else result.value = 1 / cos(operand1);
                break;
            case FUNC_CSC:
                if (IS_SIN_ZERO(operand1)) {
                    if (verbose) err(CALCULATOR, "Cosecant is infinity for given value.");
                    result = vague;
                } else result.value = 1 / sin(operand1);
                break;
            case FUNC_ARCSIN:
                if (operand1 > 1 || operand1 < -1) {
                    if (verbose) err(CALCULATOR, "given value is out of domain of inverse sine.");
                    result = vague;
                } else result.value = asin(operand1);
                break;
            case FUNC_ARCCOS:
            if (operand1 > 1 || operand1 < -1) {
                    if (verbose) err(CALCULATOR, "given value is out of domain of inverse cosine.");
                    result = vague;
                } else result.value = acos(operand1);
                break;
            case FUNC_ARCTAN:
                result.value = atan(operand1);
                break;
            case FUNC_ARCCOT:
                if (operand1 == 0) {
                    if (verbose) err(CALCULATOR, "given value is out of domain of inverse cotangent.");
                    result = vague;
                } else result.value = atan(1 / operand1);
                break;
            case FUNC_ARCSEC:
                if (operand1 == 0) {
                    if (verbose) err(CALCULATOR, "given value is out of domain of inverse secant.");
                    result = vague;
                } else result.value = acos(1 / operand1);
                break;
            case FUNC_ARCCSC:
                if (operand1 == 0) {
                    if (verbose) err(CALCULATOR, "given value is out of domain of inverse cosecant.");
                    result = vague;
                } result.value = asin(1 / operand1);
                break;
            case FUNC_LN:
                if (operand1 <= 0) {
                    if (verbose) err(CALCULATOR, "given value is out of domain of natural logarithm.");
                    result = vague;
                } else result.value = log(operand1);
                break;
            case FUNC_LOG:
                if (operand1 <= 0) {
                    if (verbose) err(CALCULATOR, "given value is out of domain of common logarithm.");
                    result = vague;
                } else result.value = log10(operand1);
                break;
            case FUNC_SQRT:
                if (operand1 < 0) {
                    if (verbose) err(CALCULATOR, "given value is out of domain of square root function.");
                    result = vague;
                } else result.value = sqrt(operand1);
                break;
            case FUNC_ABS: result.value = fabs(operand1); break;
            case FUNC_NEG: result.value = operand1 * -1; break;
        }
    }

    return (result.value == INFINITY || result.value == NAN) ? vague : result;
}
