%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbs.h"
int yylex();
void yyerror(char *);
%}

%union {
    int num;
    char caractere;
    Ident ident;
    Exp exp;
    ExpArray exp_array;
    Symb* symb;
    ArrayInfo array_info;
    WhileInst while_inst;
    IfInst if_inst;
}

%token COMMENT CONST IF ELSE WHILE PRINT READC READE RETURN
%token ASSIGN LPAREN RPAREN SEMI COMMA LBRACK RBRACK LBRACE RBRACE

%token <num> NUM VOID TYPE DIVSTAR ADDSUB ORDER EQ AND OR NOT

%token <ident> IDENT
%token <caractere> CARACTERE

%type <ident> Declarateur
%type <exp> Litteral LValue Exp EB TB FB M E T F
%type <num> NombreSigne

%type <array_info> Tableau
%type <exp_array> Indexer Arguments ListExp
%type <while_inst> WHILE
%type <if_inst> IF

/* precedencies and associativities */
%left COMMA
%right ASSIGN
%left OR
%left AND
%left EQ
%left ORDER
%left ADDSUB
%left DIVSTAR
%right NOT
%left LPAREN RPAREN LBRACK RBRACK

%expect 6
%%

Prog: DeclConsts DeclVars DeclFoncts
    | error
    ;

DeclConsts:
      DeclConsts CONST ListConst ';'
    | DeclConsts error
    |
    ;

ListConst:
      ListConst ',' IDENT '=' Litteral { push_const($3, &$5); }
    | IDENT '=' Litteral { push_const($1, &$3); }
    ;

Litteral:
       NombreSigne { expression_make(&$$, TINT, $1); }
    |  CARACTERE { expression_make(&$$, TCHAR, $1); }
    ;

NombreSigne:
       NUM { $$ = $1; }
    |  ADDSUB NUM { $$ = ($1 == OP_SUB) ? -$2 : $2; }
    ;

DeclVars:
       DeclVars TYPE ST Declarateurs ';'
    |  DeclVars error ';'
    |
    ;

Declarateurs:
       Declarateurs ',' Declarateur
    |  Declarateur
    ;

Declarateur:
       IDENT {
        strcpy($$, $1);
        push_var($1, currtype, NULL);
    }
    |  IDENT Tableau {
        strcpy($$, $1);
        push_var($1, currtype, &$2);
    }
    ;

Tableau:
	  '[' NUM ']' {
          $$.size = $2;
          $$.dims_size = 1;
          $$.dims[0] = $2;
    }
    |  Tableau '[' NUM ']' {
        $$.size = $1.size * $3;
        $$.dims_size++;
        $$.dims[$$.dims_size - 1] = $3;
    }
	;

ST: 
    { currtype = $<num>0; };

DeclFoncts:
       DeclFoncts DeclFonct 
    |  DeclFonct
    | 
    ;

DeclFonct:
       EnTeteFonct Corps { pop_func(); }
    ;

EnTeteFonct:
       TYPE ST IDENT '(' { push_func($3, $1); } Parametres { scope = LOCAL_SCOPE; } ')'
    |  VOID IDENT    '(' { push_func($2, $1); } Parametres { scope = LOCAL_SCOPE; } ')'
    |  EnTeteFonct error
    ;

Parametres:
       VOID
    |  ListTypVar
    |
    ;

ListTypVar:
       ListTypVar ',' TYPE ST Declarateur
    |  TYPE ST Declarateur
    ;

Corps: '{' DeclConsts DeclVars {
        emit_function_enter();
    } SuiteInstr '}'
    ;

SuiteInstr:
       SuiteInstr Instr 
    |  ;

Instr:
      Exp ';'                          { }
    | ';'                              { }
    | RETURN Exp ';'                   { emit_instr_return(&$2); } 
    | RETURN ';'                       { emit_instr_return(NULL); }
    | READE '(' IDENT ')' ';'          { emit_instr_reade($3); }
    | READC '(' IDENT ')' ';'          { emit_instr_readc($3); }
    | PRINT '(' Exp')' ';'             { emit_instr_print(&$3); }
    | IF '(' Exp { emit_instr_if(&$1, &$3, false); } ')' Instr { emit_outer_if(&$1); }
    | WHILE { emit_instr_while(&$1); } '(' Exp ')' { emit_check_while(&$1, &$4); } Instr { emit_outer_while(&$1); }
    | '{' SuiteInstr '}'
    | error 
    ;


Exp:  LValue {} '=' Exp { emit_instr_assign(&$1, &$4); $$ = $4; }
    |  EB
    ; 
EB:  EB OR TB   { emit_operation($2, &$1, &$3, &$$); }
    |  TB
    ;
TB:  TB AND FB  { emit_operation($2, &$1, &$3, &$$); }
    |  FB
    ;
FB:  FB EQ M    { emit_operation($2, &$1, &$3, &$$); }
    |  M
    ;
M:  M ORDER E   { emit_operation($2, &$1, &$3, &$$); }
    |  E
    ;
E:  E ADDSUB T  { emit_operation($2, &$1, &$3, &$$); }
    |  T
    ;    
T:  T DIVSTAR F { emit_operation($2, &$1, &$3, &$$); }
    |  F
    ; 
F:  ADDSUB F    { emit_operation($1, &ZERO, &$2, &$$); }
    | NOT F     { emit_operation($1, &ZERO, &$2, &$$); }
    | '(' Exp ')' { $$ = $2; }
    | LValue    { $$ = $1; }
    | NUM       { expression_make(&$$, TINT, $1); }
    | CARACTERE { expression_make(&$$, TCHAR, $1); }
    | IDENT '(' Arguments ')' {
        $$.args = &$3;
        $$.symb = find_symb($1);
        $$.indexer = NULL;
        expression_check(&$$);
        emit_instr_call(&$$);
    }
    ;
LValue:
      IDENT Indexer { 
        $$.args = NULL;
        $$.indexer =  &$2;
        $$.symb = find_symb($1);
        expression_check(&$$);
    }  
    | IDENT {
        $$.args = NULL;
        $$.indexer = NULL;
        $$.symb = find_symb($1);
        expression_check(&$$);
    } 
    ;

Indexer:
      Indexer '[' Exp ']' {
        $$.size++;
        $$.items[$$.size - 1] = $3;
    }
    | '[' Exp ']' {
        $$.size = 1;
        $$.items[0] = $2;
    }
    ;

Arguments:
      ListExp
    | { $$.size = 0; };

ListExp:
     ListExp ',' Exp {
        $$.size++;
        $$.items[$$.size - 1] = $3;
     }
    |  Exp {
        $$.size = 1;
        $$.items[0] = $1;
    }
    ;
%%

void yyerror(char* s){
    print_error(s);
}

int main(int argc, char** argv) {

    expression_make(&ZERO, TINT, 0);
    expression_make(&ONE, TINT, 1);

    if (argc >= 3 && strcmp(argv[1], "-o") == 0) {
        model_create(argv[2]);
    } else {
        model_create("../nasm/prog.asm");
    }

    emit("section .data ; global variables ");
    
    yyparse();

    print_errors();
    if (errcount == 0)
        print_symbs();

    free(symbs);
    if (output) {
        fclose(output);
    }
    return !(errcount == 0);
}
