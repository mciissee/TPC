/* module for  global variables and types of the program */

#ifndef __MODEL_H__
#define __MODEL_H__

#include <stdio.h>

#define true 1
#define false 0

#define STACK_OFFSET 8

/* global symbols scope */
#define GLOBAL_SCOPE 0

/* param symbols scope */
#define PARAM_SCOPE 1

/* local symbols scope */
#define LOCAL_SCOPE 2

/* maximum length of type name */
#define MAX_TYPES 6

/* maximum length of symbol name */
#define MAX_NAME 32

/* maximum length of an address */
#define MAX_ADDRESS 16

/* maximum number of parameters for a function */
#define MAX_PARAM 15

/* maximum number of dimensions for arrays*/
#define MAX_DIMS 2

/* max size of symbol table */
#define MAX_SYMBOLS 256

#define RED "\x1B[0;31m"
#define YELLOW "\x1B[0;33m"
#define RESET "\x1B[0m"

typedef char Ident[MAX_NAME];


/* The types of the language */
typedef enum {
    TCHAR,
    TINT,
    TLONG,
    TVOID,
} DataTypes;

/* Available operations in the language */
typedef enum {
    OP_AND,
    OP_OR,
    OP_NOT,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    OP_ADD,
    OP_SUB,
    OP_L,
    OP_LE,
    OP_G,
    OP_GE,
    OP_E,
    OP_NE,
} Op;

/* Array informations */
typedef struct {
    /* size of the array */
    int size;
    /* number of dimensions */
    int dims_size;
    /* dimensions of the array*/
    int dims[MAX_DIMS];
} ArrayInfo;


/* An item of the symbol table */
typedef struct symb {
    /* symbol datat type */
    DataTypes data_type;

    /* symbol scope 0:global 1:local */
    int scope;

    /* declaration line number */
    int lineno;

    /* constant value */
    int value;

    /* function stack size */
    int stacksize;

    /* is the symbol a local variable ?*/
    int is_local;
    
    /* is the symbol a function ?*/
    int is_function;

    /* is the symbol a constant ?*/
    int is_constant;
   
    /* is the symbol a function parameter */
    int is_param;

    /* array informations*/
    ArrayInfo array_info;

    /* local variable position in the stack */
    char address[MAX_ADDRESS];

    /* symbol name */
    char name[MAX_NAME];

    /* reference to a local variable function ?*/
    struct symb* func;

    /* function parameters ?*/
    struct params* params;
} Symb;


/* Parameters of a function */
typedef struct params {
    Symb items[MAX_PARAM];
    int size;
} Params;


typedef struct {
    int inner;
    int outer;
} WhileInst;

typedef struct {
    int elselabel;
    int outerlabel;
} IfInst;

/* current size of symbol table */
extern int size;

/* current scope of the parser 0 mean global and 1 local*/
extern int scope;

/* the symbol table */
extern Symb* symbs;

/* compiler output file */
extern FILE* output;

/* type of the current symbol table */
extern DataTypes currtype;

/* string representation of DataTypes enum values */
extern char TYPE_NAMES[][MAX_TYPES];

/* created */
void model_create(char* out);

#endif