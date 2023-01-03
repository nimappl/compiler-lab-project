#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#define PARSER 1003
#define CALCULATOR 1004
#define SDL 1002
#define RADIAN 1005
#define DEG 1006

typedef struct {
    enum { TT_NULL, TT_NUMBER, TT_OPERATOR, TT_VARIABLE,
           TT_FUNCTION, TT_BRACKET, TT_TERMINAL } type;
    enum {
        NUM, OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD, OP_POW, FUNC_NEG, FUNC_SIN, FUNC_COS,
        FUNC_TAN, FUNC_COT, FUNC_SEC, FUNC_ARCSIN, FUNC_ARCCOS, FUNC_CSC, FUNC_ARCTAN,
        FUNC_ARCCOT, FUNC_ARCSEC, FUNC_ARCCSC, FUNC_LOG, FUNC_LN, FUNC_SQRT, FUNC_ABS,
        OBRACK, CBRACK, VAR_E, VAR_PI, VAR_X, TERMINAL, NONE
    } name;
    double value;
    int presedence;
} token;

typedef struct {
    token s[100];
    int top;
} tstack;

typedef struct {
    int x;
    int y;
} point;

static enum {
    T_ADD, T_SUB, T_MUL, T_DIV, T_MOD, T_POW, T_SIN, T_COS, T_TAN, T_COT, T_SEC,
    T_CSC, T_ARCSIN, T_ARCCOS, T_ARCTAN, T_ARCCOT, T_ARCSEC, T_ARCCSC, T_NEG, T_ABS, T_LOG,
    T_LN, T_SQRT, T_OBRACK, T_CBRACK, T_VAR_E, T_VAR_PI, T_VAR_X, T_TERMINAL, T_NULL
} tokens;

static const token keywords[] = {
    { TT_OPERATOR, OP_ADD, 0, 1 },      // T_ADD        0
    { TT_OPERATOR, OP_SUB, 0, 1 },      // T_SUB        1
    { TT_OPERATOR, OP_MUL, 0, 2 },      // T_MUL        2
    { TT_OPERATOR, OP_DIV, 0, 2 },      // T_DIV        3
    { TT_OPERATOR, OP_MOD, 0, 3 },      // T_MOD        4
    { TT_OPERATOR, OP_POW, 0, 3 },      // T_POW        5
    { TT_FUNCTION, FUNC_SIN, 0, 5 },    // T_SIN        6
    { TT_FUNCTION, FUNC_COS, 0, 5 },    // T_COS        7
    { TT_FUNCTION, FUNC_TAN, 0, 5 },    // T_TAN        8
    { TT_FUNCTION, FUNC_COT, 0, 5 },    // T_COT        9
    { TT_FUNCTION, FUNC_SEC, 0, 5 },    // T_SEC        10
    { TT_FUNCTION, FUNC_CSC, 0, 5 },    // T_CSC        11
    { TT_FUNCTION, FUNC_ARCSIN, 0, 5 }, // T_ARCSIN     12
    { TT_FUNCTION, FUNC_ARCCOS, 0, 5 }, // T_ARCCOS     13
    { TT_FUNCTION, FUNC_ARCTAN, 0, 5 }, // T_ARCTAN     14
    { TT_FUNCTION, FUNC_ARCCOT, 0, 5 }, // T_ARCCOT     15
    { TT_FUNCTION, FUNC_ARCSEC, 0, 5 }, // T_ARCSEC     16
    { TT_FUNCTION, FUNC_ARCCSC, 0, 5 }, // T_ARCCSC     17
    { TT_FUNCTION, FUNC_NEG, 0, 4 },    // T_NEG        18
    { TT_FUNCTION, FUNC_ABS, 0, 5 },    // T_ABS        19
    { TT_FUNCTION, FUNC_LOG, 0, 5 },    // T_LOG        20
    { TT_FUNCTION, FUNC_LN, 0, 5 },     // T_LN         21
    { TT_FUNCTION, FUNC_SQRT, 0, 5 },   // T_SQRT       22
    { TT_BRACKET, OBRACK, 0, 0 },       // T_OBRACK     23
    { TT_BRACKET, CBRACK, 0, 0 },       // T_CBRACK     24
    { TT_VARIABLE, VAR_E, M_E, 0},      // T_VAR_E      25
    { TT_VARIABLE, VAR_PI, M_PI, 0 },   // T_VAR_PI     26
    { TT_VARIABLE, VAR_X, 0, 0 },       // T_VAR_X      27
    { TT_TERMINAL, TERMINAL, 0, 0 },    // T_TERMINAL   28
    { TT_NULL, NONE, 0, 0 }             // T_NULL       29
};

extern token final_formula[200];
extern char var;

void push(token, tstack *);
token pop(tstack *);
bool lex(char input_string[]);
bool err(int, char *);
bool is_valid_number(char []);
int determine_type(char input[], int index, int var);
void to_postfix_notation(token formula[]);
bool is_mathematically_valid();
token calculate_for(double value, bool verbose);
token calculate(token operation, double operand1, double operand2, bool verbose);
void plot();
void draw_graph();
void draw_numbers();
