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
 *  Authors: S. Lombardy, N. Bedon, C. Morvan
 *
 *************************************************************************** */

%{
#include"loadprog.h"
#include"actions.h"
#include<stdlib.h>
#define RCOM1  return COM1;
#define RCOM2 return COM2;

%}
%option noyywrap

NOMBRE -?[0-9]+

%%

{NOMBRE} {yylval=atoi(yytext); return NUM;}

"NEG"  {yylval=VM_NEG; RCOM1;}
"ADD"  {yylval=VM_ADD; RCOM1;}
"SUB"  {yylval=VM_SUB; RCOM1;}
"MULT" {yylval=VM_MULT; RCOM1;} 
"DIV"  {yylval=VM_DIV; RCOM1;}
"MOD"  {yylval=VM_MOD; RCOM1;}

"EQUAL" {yylval=VM_EQUAL; RCOM1;}
"NOTEQ" {yylval=VM_NOTEQ; RCOM1;}
"LEQ"  {yylval=VM_LEQ; RCOM1;}
"LOW"  {yylval=VM_LOW; RCOM1;}
"GEQ"  {yylval=VM_GEQ; RCOM1;}
"GREAT" {yylval=VM_GREAT; RCOM1;}

"SWAP" {yylval=VM_SWAP; RCOM1;}
"PUSH" {yylval=VM_PUSH; RCOM1;}
"POP" {yylval=VM_POP; RCOM1;}
"READ" {yylval=VM_READ; RCOM1;}
"WRITE" {yylval=VM_WRITE; RCOM1;}
"READCH" {yylval=VM_READCH; RCOM1;}
"WRITECH" {yylval=VM_WRITECH; RCOM1;}
"SET"   {yylval=VM_SET; RCOM2;}
"LOAD" {yylval=VM_LOAD; RCOM1;}
"LOADR" {yylval=VM_LOADR; RCOM1;}
"SAVE" {yylval=VM_SAVE; RCOM1;}
"SAVER" {yylval=VM_SAVER; RCOM1;}
"JUMP"  {yylval=VM_JUMP; RCOM2;}
"JUMPF" {yylval=VM_JUMPF; RCOM2;}
"LABEL" {yylval=VM_LABEL; return LBL;}
"CALL"  {yylval=VM_CALL; RCOM2;}
"RETURN" {yylval=VM_RETURN; RCOM1;}

"ALLOC" {yylval=VM_ALLOC; RCOM2;}
"FREE"  {yylval=VM_FREE; RCOM2;}
"HALT" {yylval=VM_HALT; RCOM1;}
[ \t] ;
"#".* ;
"\n"  return EOL;
. fprintf(stderr,"unknown symbol:[%s]\n",yytext);

%%

