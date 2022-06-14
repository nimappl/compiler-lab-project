#include "main.h"
#define SDL_MAIN_HANDLED

token final_formula[200];

int main(int argc, char *argv[])
{
    printf("Parse and plot mathematical functions.\nPress Ctrl+C to exit.\n\n");
    while (1)
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

        if (lex(input) && is_mathematically_valid()) {
            double value;

            printf("Enter a value for %c: ", var);
            scanf("%lf", &value);
            getchar();

            token result = calculate_for(value, true);
            if (result.type != TT_NULL) printf("Result: %lf\n\n", result.value);

            plot();
        }
    }

    return 0;
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
