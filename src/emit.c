#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "emit.h"

static int next_label;
static int section_data;
static int section_text;
static int returned;

static char SIZE_OF[][10]  = {
    "byte", // char
    "dword", // int
    "qword", // long
    "NULL", // void
};

static char DEFINES[][10]  = {
    "db", // char
    "dd", // int
    "dq", // long
    "NULL", // void
};

static char PRINT_FORMAT[][10]  = {
    "$cformat", // char
    "$iformat", // int
    "$lformat", // long
    "NULL", // void
};

static void storage(DataTypes data_type, char reg, char* out) {
    switch (data_type) {
    case TCHAR:
        sprintf(out, "%cl", reg);
        break;
    case TINT:
        sprintf(out, "e%cx", reg);
        break;
    case TLONG:
        sprintf(out, "r%cx", reg);
        break;
    default:
        break;
    }
}

static int check_assignation(const Exp* variable, const Exp* expression) {
    if (!variable->symb || expression->expr_type == UNDEF)
        return false;

    if (variable->expr_type == CONST_EXPRESSION) {
        print_error("cannot change const value '%s'", variable->symb->name);
        return false;
    }

    if (variable->expr_type == FUNCTION_EXPRESSION) {
        print_error("cannot assign an expression to a function '%s'", variable->symb->name);
        return false;
    }
 
    if (variable->symb->array_info.dims_size > 0 && !variable->indexer) {
        print_error("cannot assign an expression to an array '%s'", variable->symb->name);
        return false;
    }

    if (variable->data_type == TCHAR && expression->data_type == TINT) {
        print_warning("incompatible conversion assigning to 'char' from 'int'");
    } else if (variable->data_type == TCHAR && expression->data_type == TLONG) {
        print_warning("incompatible conversion assigning to 'char' from 'long'");
    } else if (variable->data_type == TINT && expression->data_type == TLONG) {
        print_warning("incompatible conversion assigning to 'int' from 'long'");
    }

    return true;
}

static void check_section_data() {
    if (!section_data) {
        section_data = true;
        emit("  $iformat: db \"%%d\", 0");
        emit("  $cformat: db \"%%c\", 0");
        emit("  $lformat: db \"%%ld\", 0");
    }
}

static void check_section_text() {
    if (!section_text) {
        section_text = true;
        emit("\nsection .text");
        emit("extern scanf");
        emit("extern printf");
        emit("global _start\n");
    }
}

static void emit_pop(int n, const char* reg) {
    int i;
    for (i = 0; i < n; i++) {
        emit("  pop %s", reg);
    }
}

static char* opcode(Op op) {
    switch (op) {
        case OP_E:   return  "e";
        case OP_NE:  return  "ne";
        case OP_L:   return  "l";
        case OP_LE:  return  "le";
        case OP_G:   return  "g";
        case OP_GE:  return  "ge";
        case OP_ADD: return  "add";
        case OP_SUB: return  "sub";
        case OP_MUL: return  "imul";
        case OP_DIV:
        case OP_MOD:
            return "idiv";
        default: return "";
    }
}

/** move the address of 'symb' to rcx */
static void read_symb_addr(const Symb* symb) {
    assert(symb != NULL);
    if (symb->is_local) {
        emit("  %s rcx, %s", symb->is_param ? "mov" : "lea", symb->address);
    } else {
        emit("  mov rcx, %s", symb->name);
    }
}

/** converts the indexer expression to an array index and move it to rcx */
static void read_indexer(const Exp* expression) {
    assert(expression != NULL);
    int i, dimension, usage;

    dimension = expression->symb->array_info.dims_size;
    if (dimension != expression->indexer->size) {
        // ex: array[N][M] used as array[N];
        print_error("'%s' should be indexed with %d dimension(s) ", expression->symb->name, dimension);
        return;
    }

    for (i = 0; i < expression->indexer->size; i++) {
        read_expression(&expression->indexer->items[i]);
        emit("  push rcx    ; push indexer[%d]", i + 1);
    }

    switch (expression->indexer->size) {
        case 1:
            emit("  pop rcx ; pop indexer[1]");
            break;
        case 2:
            // http://www.guideforschool.com/625348-memory-address-calculation-in-an-array/
            // https://www.techoschool.com/MCA/Data-Structures-using-C/UNIT-1-Arrays_Address-Calculation
            emit("  ; [i][j] to 1D dimension [i] = i * j + N");
            emit("  pop rbx ; col"); // col
            emit("  pop rcx ; row"); // row
            emit("  imul rcx, %d", expression->symb->array_info.dims[1]);
            emit("  add rcx, rbx");
        break;
        default:
            print_error("'%d' dimensions arrays not supported", usage);
            break;
    }
}

/** move the expression to rcx */
void read_expression(const Exp* expression) {
    assert(expression != NULL);
    char rcx[5];
    char rax[5];
    storage(expression->data_type, 'c', rcx);
    switch (expression->expr_type) {
        case CONST_EXPRESSION:
        case LVALUE_EXPRESSION:
            if (expression->expr_type == CONST_EXPRESSION) {
                emit("  mov %s, %d", rcx, expression->value);
                return;
            }
            if (expression->symb->is_constant) {
                emit("  mov %s, %d", rcx, expression->symb->value);
            } else if (expression->symb->array_info.dims_size > 0) {
                read_symb_addr(expression->symb);
            } else {
                emit("  mov %s, %s", rcx, expression->symb->address);
            }
            break;
        case ARRAY_EXPRESSION:
            read_indexer(expression);
            emit("  mov rbx, rcx");
            read_symb_addr(expression->symb);
            if (expression->symb->is_local) {
                if (expression->symb->is_param) {
                    emit("  mov rdx, %s", expression->symb->address);
                } else {
                    emit("  lea rdx, %s", expression->symb->address);
                }
                emit("  mov %s, [rdx + rbx * %d]", rcx, data_size(expression->data_type));
            } else {
                emit("  mov %s, [%s + rbx * %d]", rcx, expression->symb->name, data_size(expression->data_type));
            }
            break;
        // OPERATIONS ARE PUSHED ON THE STACK ONCE CALCULATED
        case OPERATION_EXPRESSION:
            emit("  pop rcx");
            break;
        // FUNCTIONS RET VALUE ARE STORED IN RAX
        case FUNCTION_EXPRESSION:
            storage(expression->data_type, 'a', rax);
            emit("  mov %s, %s", rcx, rax);
            break;
        default:
        print_error("cannot read void expression");
        break;
    }
}

void emit(const char* message, ...) {
    va_list args;
    va_start (args, message);
    vfprintf (output, message, args);
    va_end (args);
    fprintf(output, "\n");
}

void emit_symb(const Symb* symb) {
    assert(symb != NULL);
    if (symb->scope == GLOBAL_SCOPE) {
        char times[10];
        strcpy(times, "");
        check_section_data();
        if (symb->array_info.size > 0) {
            sprintf(times, "times %d", symb->array_info.size);
        }
        if (symb->is_constant) {
            emit("  %s: equ %d; const", symb->name, symb->value);
        } else {
            emit("  %s: %s %s %d", symb->name, times, DEFINES[symb->data_type], symb->value);
        }
    } else if (symb->is_param) {
        emit("  ; param %s:  mov %s", symb->name, symb->address);
    } else {
        emit("  ; local var %s: %s", symb->name, symb->address);
        emit("  mov %s %s, %d; %s", SIZE_OF[symb->data_type], symb->address, symb->value, symb->name);
    }
}

void emit_function_enter() {
    check_section_data();
    check_section_text();

    returned = 0;
    Symb* func = peek_func();
    check_section_data();
    check_section_text();

    if (strcmp(func->name, "main") == 0) {
        emit("_start: ");
    } else {
        emit("%s:", func->name);
    }
    emit("  push rbp");
    emit("  mov rbp, rsp");
    if (func->stacksize > 0) {
        emit("  sub rsp, %d", func->stacksize);
    }
    emit("");
    int i;
    for (i = i + 1; i < size; i++) {
        emit_symb(&symbs[i]);
    } 
}

void emit_function_exit() {
    if (!returned) {
        emit_instr_return(NULL);
    }
}

void emit_instr_return(const Exp* expression) {
    emit("");
    returned = 1;

    Symb* func = peek_func();
    if (expression) {
        read_expression(expression);
        emit("  mov rax, rcx");
    }
    emit("  mov rsp, rbp");
    emit("  pop rbp");
    if (strcmp(func->name, "main") == 0) {
        emit("  mov eax, 1");
        emit("  mov ebx, eax");
        emit("  int 80h");
    } else {
        emit("  ret\n");
    }
}

void emit_instr_print(const Exp* expression) {
    read_expression(expression);
    switch (expression->data_type) {
        case TINT:
            emit("  mov rsi, rcx");
            emit("  mov rdi, $iformat");
            emit("  mov rax, 0");
            emit("  call printf");
        break;
        case TCHAR:
            emit("  mov rsi, rcx"); 
            emit("  mov rdi, $cformat");
            emit("  mov rax, 0");
            emit("  call printf");
        break;
        case TLONG:
            emit("  mov rsi, rcx");
            emit("  mov rdi, $lformat");
            emit("  mov rax, 0");
            emit("  call printf");
        break;
        default: break;
    }
}

void emit_instr_reade(const Ident ident) {
    find_symb(ident);
    emit("  push %d", ident);
    emit("  push $iformat");
    emit("  call scanf");
    emit("  pop rax");
    emit("  pop rax");
}

void emit_instr_readc(const Ident ident) {
    find_symb(ident);
    emit("  push %d", ident);
    emit("  push $cformat");
    emit("  call scanf");
    emit("  pop rax");
    emit("  pop rax");
}

void emit_operation(Op op, const Exp* left, const Exp* right, Exp* result) {
    result->expr_type = UNDEF;
    if (left->expr_type == UNDEF || right->expr_type == UNDEF) { 
        return;
    }

    if (left->data_type == TVOID || right->data_type == TVOID) { 
        print_error("void type cannot be used here");
        return;
    }

    result->value = 0;
    result->args = NULL;
    result->symb = NULL;
    result->indexer = NULL;
    result->data_type = left->data_type == TLONG || right->data_type == TLONG ? TLONG : TINT;
    result->expr_type = OPERATION_EXPRESSION;

    read_expression(left);
    emit("  mov rax, rcx");
    read_expression(right);

    switch (op) {
        case OP_ADD:
        case OP_SUB:
        case OP_MUL:
        case OP_DIV:
        case OP_MOD:
            if (op == OP_MOD || op == OP_DIV) {
                emit("  mov rdx, 0");
                emit("  idiv ecx");
            } else {
                emit("  %s rax, rcx", opcode(op));
            }
            if (op == OP_MOD) {
                emit("  push rdx");
            } else {
                emit("  push rax");
            }
            break;
        case OP_L:
        case OP_G:
        case OP_E:
        case OP_LE:
        case OP_GE:
        case OP_NE:
        case OP_NOT:
            emit("  cmp rax, rcx");
            emit("  set%s cl", opcode(op));
            emit("  and cl, 1");
            emit("  movzx rax, cl");
            emit("  push rax");
            break;
        default:
        emit("  ; operations && and || not implemented");
        emit("  push 1");
        break;
    }

}

void emit_instr_call(const Exp* expression) {
    assert(expression != NULL);
    Symb* function = expression->symb;
    ExpArray* args = expression->args;
    int i;
    for (i = args->size - 1; i >= 0 ; i--) {
        read_expression(&args->items[i]);
        emit("  push rcx");
    }
    emit("  call  %s", function->name);
    emit_pop(args->size, "rbx");
}

void emit_instr_assign(const Exp* lvalue, const Exp* expression) {
    if (!check_assignation(lvalue, expression))
        return;

    read_expression(expression);
    if (lvalue->indexer) {
        emit("  push rcx");
        read_indexer(lvalue);
        emit("  mov rbx, rcx");
        emit("  pop rax");
        read_symb_addr(lvalue->symb);
        // rcx = array address
        // rbx = indexer
        // rax = expression
        emit("  mov [rcx + rbx * %d], rax", data_size(lvalue->data_type));
    } else {
        char rcx[5];
        storage(lvalue->data_type, 'c', rcx);
        emit("  mov %s, %s", lvalue->symb->address, rcx);
    }
}

void emit_instr_while(WhileInst* instruction) {
    emit(".LBL%d:   ; LOOP INNER", next_label);
    instruction->inner = next_label;
    instruction->outer = next_label + 1;
    next_label += 2;
}

void emit_check_while(WhileInst* instruction, Exp* expression) {
    read_expression(expression);
    emit("  cmp rcx, 0");
    emit("  je .LBL%d", instruction->outer);
}

void emit_outer_while(WhileInst* instruction) {
    emit("  jmp .LBL%d; REPEAT", instruction->inner);
    emit(".LBL%d:   ; LOOP OUTER", instruction->outer);
}

void emit_instr_if(IfInst* instruction, Exp* expression, int withElse) {
    read_expression(expression);
    instruction->elselabel = next_label;
    instruction->outerlabel = withElse ? next_label + 1 : next_label;
    emit("  cmp rcx, 0");
    emit("  je .LBL%d", withElse ? instruction->elselabel : instruction->outerlabel);
    next_label += withElse ? 2 : 1;
}

void emit_instr_else(IfInst* instruction) {
    emit("  jmp .LBL%d", instruction->outerlabel);
    emit(".LBL%d:", instruction->elselabel);
}

void emit_outer_if(IfInst* instruction) {
    emit(".LBL%d:", instruction->outerlabel);
}
