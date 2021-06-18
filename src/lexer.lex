%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symbs.h"
#include "parser.h"
extern int lineno;
%}

%option noinput nounput noyywrap 
%x COMMENT

%%

"//".*
"/*"					{  BEGIN(COMMENT); }
<COMMENT>"*/" 		    {  BEGIN(INITIAL); }
<COMMENT>[^*\n]+		
<COMMENT>"*"			
<COMMENT>"\n"		    { lineno += 1; }

"\n"			        { lineno += 1; }
[ \t\r\f]+		

[0-9]+ 					{ sscanf(yytext,"%d",&yylval.num); return NUM; }

&&                      { yylval.num = OP_AND; return AND; }
"||"                    { yylval.num = OP_OR;  return OR; }
"!"                     { yylval.num = OP_NOT; return NOT; }
"*"                     { yylval.num = OP_MUL; return DIVSTAR; }
"/"                     { yylval.num = OP_DIV; return DIVSTAR; }
%                       { yylval.num = OP_MOD; return DIVSTAR; }
"+"                     { yylval.num = OP_ADD; return ADDSUB; }
-                       { yylval.num = OP_SUB; return ADDSUB; }
"<"                     { yylval.num = OP_L;   return ORDER; }
"<="                    { yylval.num = OP_LE;  return ORDER; }
">"                     { yylval.num = OP_G;   return ORDER; }
>=                      { yylval.num = OP_GE;  return ORDER; }
==                      { yylval.num = OP_E;   return EQ; }
!=                      { yylval.num = OP_NE;  return EQ; }

"="						{ return yytext[0]; return ASSIGN; }
"("				     	{ return yytext[0]; return LPAREN; }
")"						{ return yytext[0]; return RPAREN; }
";"						{ return yytext[0]; return SEMI;}
","						{ return yytext[0]; return COMMA;}
"["						{ return yytext[0]; return LBRACK; }
"]"						{ return yytext[0]; return RBRACK; }
"{"						{ return yytext[0]; return LBRACE; }
"}"						{ return yytext[0]; return RBRACE; }

void                    { yylval.num = TVOID; return VOID; }
int                     { yylval.num = TINT;  return TYPE; }
char                    { yylval.num = TCHAR; return TYPE; }
long                    { yylval.num = TLONG; return TYPE; }

const 					{ return CONST; } 
if 						{ return IF; } 
else 					{ return ELSE; }
while 					{ return WHILE; }
print 					{ return PRINT; }
readc 					{ return READC; }
reade					{ return READE; }
return 					{ return RETURN; }

[a-zA-Z_][a-zA-Z0-9_]*  { strcpy(yylval.ident, yytext); return IDENT; }
(\'\\[nftrbv]\')        { yylval.caractere = 10; return CARACTERE; }
(\'\\0\')               { yylval.caractere = 0; return CARACTERE; }
(\'[ -~]\')             { yylval.caractere = yytext[1]; return CARACTERE; }

. 						{ fprintf(stderr, "Unknown symbol : %c\n",yytext[0]); }

%%
