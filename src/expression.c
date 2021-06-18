#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "expression.h"

Exp ONE;
Exp ZERO;

void expression_check(Exp* expression) {
    int i;
    assert(expression != NULL);
    expression->expr_type = UNDEF;
    if (expression->symb) {
        expression->data_type = expression->symb->data_type;
        if (expression->indexer) {
            if (expression->symb->array_info.size <= 0) {
                print_error("'%s' cannot be indexed as an array", expression->symb->name);
                return;
            }
            if (expression->indexer) {
                for (i = 0; i < expression->indexer->size; i++) {
                    if (expression->indexer->items[i].data_type == TVOID) {
                        print_error("dimension '%d' of %s is not an integer", i, expression->symb->name);
                        return;
                    }
                }
            }
            expression->expr_type = ARRAY_EXPRESSION;
        } else if (expression->symb->is_constant) {
            expression->value = expression->symb->value;
            expression->expr_type = CONST_EXPRESSION;
        } else if (expression->symb->is_function) {
            if (!expression->args) {
                print_error("'%s' must be used with () operator", expression->symb->name);
                return;
            }

            int params_size = expression->symb->params->size;
            if (expression->args->size != params_size) {
                print_error("'%s' must be called with '%d' args but got '%d'", expression->symb->name, params_size, expression->args->size);
                return;
            }
            for (i = 0; i < params_size; i++) {
                if (expression->args->items[i].data_type == TVOID) {
                    print_error("args '%d' of '%s' is not valid", i + 1, expression->symb->name);
                    return;
                }
            }
            expression->expr_type = FUNCTION_EXPRESSION;
        } else {
            expression->expr_type = LVALUE_EXPRESSION;
        }
    }
}

void expression_make(Exp* expression, DataTypes data_type, int value) {
    assert(expression != NULL);
    
    expression->data_type = data_type; 
    expression->expr_type = CONST_EXPRESSION;
    expression->value = value;
    expression->symb = NULL;
    expression->args = NULL;
    expression->indexer = NULL;
}

void expression_print(const Exp* expression) {
    assert(expression != NULL);
    
    switch (expression->expr_type) {
        case CONST_EXPRESSION:
        fprintf(stderr, "%d\n", expression->value);
        break;
        case LVALUE_EXPRESSION:
        fprintf(stderr, "%s\n", expression->symb->name);
        break;
        case ARRAY_EXPRESSION:
        fprintf(stderr, "%s[] * %d\n", expression->symb->name, expression->indexer->size);
        break;
        case FUNCTION_EXPRESSION:
        fprintf(stderr, "%s()\n", expression->symb->name);
        break;
        case OPERATION_EXPRESSION:
        fprintf(stderr, "OP\n");
        break;
        default:  fprintf(stderr, "undef\n"); break;
    }
}

ExpArray* exp_array_copy(ExpArray* source) {
    if (!source)
        return NULL;
    ExpArray* copy = malloc(sizeof(ExpArray));
    if (!copy)
        return copy;
    int i;
    for (i = 0; i < source->size; i++) {
      expression_copy(&source->items[i], &copy->items[i]);
    }
    copy->size = source->size;
    return copy;
}

void expression_copy(const Exp* source,  Exp* dest) {
    if (source == NULL) {
        return;
    }
    dest->value = source->value;
    dest->data_type = source->data_type;
    dest->expr_type = source->expr_type;
    dest->indexer = NULL;
    dest->args = NULL;

    if (source->indexer && source->indexer->size > 0) {
        dest->indexer = exp_array_copy(source->indexer);
    }

    if (source->args && source->args->size > 0) {
        dest->args = exp_array_copy(source->args);
    }
}