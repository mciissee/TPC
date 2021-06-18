#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include "model.h"
#include "print.h"

typedef enum {
    UNDEF,
    LVALUE_EXPRESSION,
    FUNCTION_EXPRESSION,
    ARRAY_EXPRESSION,
    CONST_EXPRESSION,
    OPERATION_EXPRESSION,
} Expressions;

/* Representation of an expression */
typedef struct exp {
    /* expression data type */
	DataTypes data_type;

    /* expression type */
    Expressions expr_type;

    /* constant expression value */
	int value;

    /* lvalue expression */
    struct symb* symb;

    /* function arguments */
    struct exp_array* args;

    /* array subscript expression */
    struct exp_array* indexer;
} Exp;

/* Array indexer informations */
typedef struct exp_array {
    /* number of expressions */
    int size;
    /* the expressions */
    Exp items[MAX_PARAM];
} ExpArray;

/* 1 const expression */
extern Exp ONE;

/* 0 const expression */
extern Exp ZERO;

/* 
 * Checks if the èxpression` is valid lvalue expression.
 * If the expression is valid, the field expression.expr_type will be sets to UNDEF.
 * @param expression the expression to check.
 */
void expression_check(Exp* expression);

/* 
 * Makes new constant expression.
 * @param expression the expression.
 * @param data_type the type of the constant.
 * @param value the value of the constant.
 */
void expression_make(Exp* expression, DataTypes data_type, int value);

/* 
 * Prints the expression.
 * @param expression the expression
 */
void expression_print(const Exp* expression);

/*
 * Assigns `expression` to `symb`.
 * @param name the name of the symbol
 * @param expression the expression to assign.
 * */
void expression_assign(Symb* symb, const Exp* expression);

void expression_copy(const Exp* source, Exp* dest);
ExpArray* exp_array_copy(ExpArray* source);
#endif