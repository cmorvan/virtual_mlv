/* ***************************************************************************
 * This file is part of virtual_mlv.
 *
 * virtual_mlv is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * virtual_mlv is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with virtual_mlv.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 *
 *  Authors: S. Lombardy, N. Bedon, C. Morvan, W. Hay, Q. Campos, J. Mangue
 *
 *************************************************************************** */

%{
extern int fileno(FILE *);
#include "loadprog.h"
#include "opcode.h"
#include <stdlib.h>
extern int nerr;
%}

%option noinput
%option nounput
%option noyywrap
%option yylineno

NUMBER -?[0-9]+

%%

{NUMBER}    { yylval = atoi(yytext);    return NUM; }

"NOP"       { yylval = VM_NOP;      return COM1; }
"NEG"       { yylval = VM_NEG;      return COM1; }
"ADD"       { yylval = VM_ADD;      return COM1; }
"SUB"       { yylval = VM_SUB;      return COM1; }
"MUL"       { yylval = VM_MUL;      return COM1; }
"DIV"       { yylval = VM_DIV;      return COM1; }
"MOD"       { yylval = VM_MOD;      return COM1; }

"EQUAL"     { yylval = VM_EQUAL;    return COM1; }
"NOTEQ"     { yylval = VM_NOTEQ;    return COM1; }
"LEQ"       { yylval = VM_LEQ;      return COM1; }
"LESS"      { yylval = VM_LESS;     return COM1; }
"GEQ"       { yylval = VM_GEQ;      return COM1; }
"GREATER"   { yylval = VM_GREATER;  return COM1; }

"SWAP"      { yylval = VM_SWAP;     return COM1; }
"PUSH"      { yylval = VM_PUSH;     return COM1; }
"POP"       { yylval = VM_POP;      return COM1; }
"READ"      { yylval = VM_READ;     return COM1; }
"WRITE"     { yylval = VM_WRITE;    return COM1; }
"READCH"    { yylval = VM_READCH;   return COM1; }
"WRITECH"   { yylval = VM_WRITECH;  return COM1; }
"SET"       { yylval = VM_SET;      return COM2; }
"LOAD"      { yylval = VM_LOAD;     return COM1; }
"LOADR"     { yylval = VM_LOADR;    return COM1; }
"SAVE"      { yylval = VM_SAVE;     return COM1; }
"SAVER"     { yylval = VM_SAVER;    return COM1; }
"JUMP"      { yylval = VM_JUMP;     return COM2; }
"JUMPF"     { yylval = VM_JUMPF;    return COM2; }
"LABEL"     { yylval = VM_LABEL;    return LBL; }
"CALL"      { yylval = VM_CALL;     return COM2; }
"RETURN"    { yylval = VM_RETURN;   return COM1; }

"ALLOC"     { yylval = VM_ALLOC;    return COM2; }
"FREE"      { yylval = VM_FREE;     return COM2; }
"HALT"      { yylval = VM_HALT;     return COM1; }
"TOPST"     { yylval = VM_TOPST;    return COM1; }
[ \t] ;
"#".* ;
"\n"        return EOL;
.   { fprintf(stderr, "line %d: unknown symbol `%s'\n", yylineno, yytext);
      nerr++;
    }

%%
