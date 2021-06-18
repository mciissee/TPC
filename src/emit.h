#ifndef __COMPILER_H__
#define __COMPILER_H__

#include "symbs.h"
#include "expression.h"

void emit(const char* message, ...);

void emit_symb(const Symb* symb);
void emit_function_enter();
void emit_function_exit();

void emit_instr_return(const Exp* expression);
void emit_operation(Op op, const Exp* left, const Exp* right, Exp* result);
void emit_instr_assign(const Exp* lvalue, const Exp* expression);
void emit_instr_print(const Exp* expression);
void emit_instr_reade(const Ident ident);
void emit_instr_readc(const Ident ident);
void emit_instr_call(const Exp* expression);
void emit_instr_while(WhileInst* instruction);
void emit_check_while(WhileInst* instruction, Exp* expression);
void emit_outer_while(WhileInst* instruction);

void emit_instr_if(IfInst* instruction, Exp* expression, int withElse);
void emit_instr_else(IfInst* instruction);
void emit_outer_if(IfInst* instruction);
void read_expression(const Exp* expression);

#endif
