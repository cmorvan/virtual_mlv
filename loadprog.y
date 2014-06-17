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
 *  Authors: S. Lombardy, N. Bedon, C. Morvan, G. Fuhs, W. Hay
 *
 *************************************************************************** */

%{
#include <stdio.h>
#include <stdlib.h>
#include "opcode.h"
#include "array.h"

int yylval;
int yylex(void);
int yyerror(const char *);
extern FILE *yyin;

typedef struct Instruction Instruction;
struct Instruction {
    Opcode opcode;
    int arg;
    int has_arg;
    Instruction *next_instruction;
};

Instruction *new_inst(Instruction *old) {
    Instruction *inst = malloc(sizeof(*inst));
    inst->next_instruction = old;
    return inst;
}

/* Labels are not stored in a standard array anymore, but in a dynamic array. */
Array *labels = NULL;

int prognum = 0;
Opcode *prog;
int prog_length;
Instruction *instructions = NULL;

%}

%token NUM COM1 COM2 LBL
%token EOL
%start input

%%

input : line input {}
      | {return 0;}

line : EOL {}
     | COM1 EOL { instructions = new_inst(instructions);
                  instructions->opcode = $1;
		          instructions->has_arg = 0;
                  prognum++;
     }
     | COM2 NUM EOL { instructions = new_inst(instructions);
                      instructions->opcode = $1;
                      instructions->arg = $2;
                      instructions->has_arg = 1;
                      prognum += 2;
     }
     | LBL NUM EOL { add_value_at_index(labels, prognum, $2); }

%%

int yyerror(const char *msg) {
    fprintf(stderr, "\n%s\n", msg);
    return 0;
}

int loadprog(char *src) {
    Instruction *inst_list;
    int i;
    labels = new_array();
    yyin = fopen(src, "r");
    yyparse();
    /* Global variable : prog_length (from vm.c) */
    prog_length = prognum;
    inst_list = instructions;
    /* Global variable : prog (from vm.c) */
    prog = malloc(prog_length * sizeof(*prog));
    i = prog_length;
    while (instructions != NULL) {
        if (instructions->has_arg) {
            if (instructions->opcode == VM_CALL
                    || instructions->opcode == VM_JUMP
                    || instructions->opcode == VM_JUMPF) {
                prog[--i] = get_value_at_index(labels, instructions->arg);
            }
            else {
                prog[--i] = instructions->arg;
            }
        }
        prog[--i] = instructions->opcode;
        inst_list = instructions;
        instructions = instructions->next_instruction;
        free(inst_list);
    }
    free_array(labels);
    return 0;
}

	
