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
#include<stdlib.h>
#include<stdio.h>
#include"actions.h"
   int yylval ;
    int yylex( );
    int yyerror(char*);
    extern FILE *yyin;

typedef struct INST{
  int com;
  int arg;
  int has_arg;
  struct INST* next_inst;
} inst;

inst* new_inst(inst* old){
  inst* l=(inst*)malloc(sizeof(inst));
  l->next_inst=old;
  return l;
}

int labels[100];
int prognum=0;
int *prog;
int prog_length;
inst* list=NULL;

%}


%token NUM COM1 COM2 LBL
%token EOL
%start entree

%%
entree : ligne entree {}
       | {return 0;}

ligne : EOL {}
|	COM1 EOL {list=new_inst(list); 
                  list->com=$1;
		  list->has_arg=0;
                  prognum++;}
      | COM2 NUM EOL {list=new_inst(list);
		     list->com=$1; 
		     list->arg=$2;
		     list->has_arg=1;
                     prognum+=2;}
      | LBL NUM EOL {labels[$2]=prognum;}
%%

int yyerror(char *msg) {
  fprintf(stderr,"\n%s\n", msg);
  return 0;
}

int loadprog(char* src){
  inst *l;
  int i;
  yyin=fopen(src,"r");
  yyparse();
  /* Global variable : prog_length (from vm.c) */
  prog_length=prognum;
  l=list;
  /* Global variable : prog (from vm.c) */
  prog=(int*)malloc(sizeof(int)*prog_length);
  i=prog_length;
  while(list!=NULL){
    if(list->has_arg==1){
      if(list->com==VM_CALL || list->com==VM_JUMP || list->com==VM_JUMPF)
	prog[--i]=labels[list->arg];
      else
	prog[--i]=list->arg;
    }
    prog[--i]=list->com;
    l=list;
    list=list->next_inst;
    free(l);
  }
  return 0;
}

	
