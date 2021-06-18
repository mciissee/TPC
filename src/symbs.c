#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "symbs.h"

int data_size(int type) {
    switch (type) {
        case TCHAR: return 1;
        case TINT: return 4;
        case TLONG: return 8;
        case TVOID: return 0;
        default: return 0;
    }
}

Symb* find_symb(const char* name) {
    int i;
    // reverse search for locals then globals
    for (i = size - 1; i >= 0; i--) {
        if (strcmp(symbs[i].name, name) == 0)
            return &symbs[i];
    }
    print_error("use of undeclared identifier '%s'", name);
    return NULL;
}


Symb* peek_func() {
    int i = size - 1;
    while (i >= 0) {
        if (symbs[i].is_function)
            return &symbs[i];
        i--;
    }
    return NULL;
}

int push_symb(const char* name, DataTypes data_type) {
    Symb* symb = NULL;
    int i;
    for (i = size - 1; i >= 0; i--) {
        if (strcmp(symbs[i].name, name) == 0) {
            symb = &symbs[i];
            break;
        }
    }
    if (symb && symb->scope == scope) {
        print_error("redefinition of '%s'", name);
        return -1;
    }

    if (++size > MAX_SYMBOLS) {
        print_error("memory error");
        exit(EXIT_FAILURE);
    }

    symb =  &symbs[size-1];
    strcpy(symb->name, name);

    symb->data_type = data_type;
    symb->lineno = lineno;
    symb->scope = scope;
    strcpy(symb->address, "");
    symb->stacksize = 0;
    symb->array_info.size = 0;
    symb->array_info.dims_size = 0;
    symb->value = 0;
    symb->is_constant = false;
    symb->is_function = false;
    symb->is_local = false;
    symb->params = NULL;
    symb->func = NULL;
    return size - 1;
}

int push_var(const char* name, DataTypes data_type, const ArrayInfo* array_info) {
    int index = push_symb(name, data_type);
    if (index == -1)
        return -1;

    symbs[index].value = 0;
    symbs[index].is_param = scope == PARAM_SCOPE;
    symbs[index].is_local = scope != GLOBAL_SCOPE;

    if (array_info) {
        symbs[index].array_info.size = array_info->size;
        symbs[index].array_info.dims_size = array_info->dims_size;
        int i;
        for (i = 0; i < array_info->dims_size; i++) {
            symbs[index].array_info.dims[i] = array_info->dims[i];
        }
    }
    Symb* func;
    if (scope == GLOBAL_SCOPE) {
        sprintf(symbs[index].address, "[%s]", symbs[index].name);
        emit_symb(&symbs[index]);
    } else if (scope == PARAM_SCOPE) {
        func = peek_func();
        int stack = 8;
        int offset = stack + (stack * (func->params->size + 1));
        sprintf(symbs[index].address, "[rbp + %d]", offset);
        func->params->items[func->params->size++] = symbs[index];
    } else if (scope == LOCAL_SCOPE) {
        func = peek_func();
        int offset;
        offset = func->stacksize + data_size(data_type);
        sprintf(symbs[index].address, "[rbp - %d]", offset);
        
        offset = data_size(data_type);
        offset *= array_info ? array_info->size : 1;
        func->stacksize += offset;
    }

    return 1;
}

int push_const(const char* name, const Exp* expression) {
    int index = push_symb(name, expression->data_type);
    if (index == -1)
        return -1;

    symbs[index].value = expression->value;
    symbs[index].is_constant = true;   
    symbs[index].is_local = scope != GLOBAL_SCOPE;

    sprintf(symbs[index].address, "%d", expression->value);
    if (scope == GLOBAL_SCOPE)
        emit_symb(&symbs[index]);
    return 1;
}

int push_func(const char* name, DataTypes data_type) {
    int index;
    index = push_symb(name, data_type);
    symbs[index].params = (Params*) malloc(sizeof(Params));
    symbs[index].params->size = 0;
    if (index != -1) {
        symbs[index].is_function = true;
        if (strcmp(name, "main") == 0)
            mainfound = true;
        scope = PARAM_SCOPE;
        return 1;
    }
    return -1;
}

int pop_func() {
    while (symbs[size - 1].scope != GLOBAL_SCOPE)
        size--;
    scope = GLOBAL_SCOPE;
    emit_function_exit();
    return 1;
}

void print_symbs() {
    printf("%6s %6s %6s %6s %6s %s \n\n", 
        "lineno","function", "constant", "arrsize", "data_type", "name"
    );
    int i, j;
    for (i = 0; i < size; i++) {
        printf("%4d %6d %8d %8d %8s %8s", 
        symbs[i].lineno, symbs[i].is_function, symbs[i].is_constant,
        symbs[i].array_info.size, TYPE_NAMES[symbs[i].data_type], symbs[i].name);

        if (symbs[i].array_info.size > 0) {
            printf(" ");
            for (j = 0; j < symbs[i].array_info.dims_size; j++)
                printf("[%d]", symbs[i].array_info.dims[j]);
        }
        if (symbs[i].is_function && symbs[i].params->size > 0) {
            printf(" (");
            for (j = 0; j < symbs[i].params->size; j++) {
                printf("%s %s", TYPE_NAMES[symbs[j].data_type], symbs[i].params->items[j].name);
                if (j != symbs[i].params->size - 1) {
                    printf(", ");
                }
            }
            printf(")");
        }
        printf("\n");
    }
}

