#ifndef __SYMBS_H__
#define __SYMBS_H__

#include "emit.h"
#include "model.h"
#include "print.h"
#include "expression.h"

/*
 * prints the symbol table on stdout
 */
void print_symbs();


int data_size(int type);

/*
 * removes local variables from symbols table.
 * @returns 0 if an error occurs 1 otherwise.
 */
int pop_func();

/*
 * pushs a function into the symbol table.
 * 
 * @param name the name of the function.
 * @param data_type the return type of the function.
 * @returns 0 if an error occurs 1 otherwise.
 */
int push_func(const char* name, DataTypes data_type);

/*
 * pushs a constant into the symbol table.
 * @param name the name of the const.
 * @param expression the expression associated to the constant.
 * @returns 0 if an error occurs 1 otherwise.
 */
int push_const(const char* name, const Exp* expression);

/*
 * pushs a variable into the symbol table.
 * 
 * @param name the name of the variable.
 * @param data_type the type of the variable.
 * @param array_info informations in case of array variable
 * @returns 0 if an error occurs 1 otherwise.
 */
int push_var(const char* name, DataTypes data_type, const ArrayInfo* array_info);


/*
 * finds the symbol `name` from the symbol table.
 * 
 * @param name the name of the symbol.
 * @param throws a value indicating whether an error message 
 * should be displayed if the symbol is not found.
 * @returns 0 if an error occurs 1 otherwise.
 */
Symb* find_symb(const char* name);

/*
 * Peeks the function at the top of the symbol table
 */
Symb* peek_func();

#endif
