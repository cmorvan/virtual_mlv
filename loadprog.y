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

/* Loadprog() error codes. */
#define PARSE_ERROR    -1
#define LABELS_ERROR   -2
#define CODE_SEG_ERROR -3
#define NO_PROG_ERROR  -4

/* Flex and Bison declarations. */
int yylval;
int yylex(void);
int yyerror(const char *);
extern FILE *yyin;
extern int yylineno;

typedef struct Instruction {
    Opcode opcode;
    int arg;
    int has_arg;
} Instruction;

typedef struct List List;
struct List {
    Instruction instruction;
    List *next;
};

/** The dynamic array of labels. */
static Array *labels;

/** The code segment. */
extern Opcode *prog;

/** The length of the program (excluding labels). */
int prog_length = 0;

/** The list of instructions. */
static List *list;

/** The number of semantic errors. */
static int nerr;

/**
 * Creates a new list and appends another list to it. The insertion is done in
 * constant time.
 * @param next The next list of the newly created list.
 * @return The newly created list.
 */
List *new_list(List *next) {
    List *list = malloc(sizeof(*list));
    list->next = next;
    return list;
}

%}

%error-verbose
%token NUM COM1 COM2 LBL EOL
%start input

%%

input : line input {}
      | { return 0; }

line : EOL {}
     | COM1 EOL {
         list = new_list(list);
         list->instruction.opcode = $1;
         list->instruction.has_arg = 0;
         prog_length++;
     }
     | COM2 NUM EOL {
         if ($1 != VM_SET && $2 < 0) {
             nerr++;
             fprintf(stderr, "line %d: %s expects an unsigned integer as "
                "argument but is here used with a signed integer\n",
                yylineno, opcode_to_string($1));
         }
         list = new_list(list);
         list->instruction.opcode = $1;
         list->instruction.arg = $2;
         list->instruction.has_arg = 1;
         prog_length += 2;
     }
     | LBL NUM EOL {
         if ($2 < 0) {
             nerr++;
             fprintf(stderr, "line %d: LABEL expects an unsigned integer as "
                "argument but is here used with a signed integer\n", yylineno);
         }
         /* The casts are safe since both integers are strictly positive. */
         add_value_at_index(labels, (unsigned) prog_length, (unsigned) $2);
     }

%%

/**
 * The bison error reporting function. Prints an error message preceded by the
 * line number.
 * @param msg The error message to be printed.
 * @return Always 0.
 */
int yyerror(const char *msg) {
    fprintf(stderr, "line %d: %s\n", yylineno, msg);
    return 0;
}

/**
 * Prints an error message according to the value returned by the @p loadprog
 * function.
 * @param errno The error code whose message is to be printed.
 */
void print_load_error(int errno) {
    switch (errno) {
    case 0: /* Success. */ break;
    case PARSE_ERROR:
        fprintf(stderr, "error: yyparse() failed\n");
        break;
    case LABELS_ERROR:
        fprintf(stderr, "error: the labels array could not be allocated\n");
        break;
    case CODE_SEG_ERROR:
        fprintf(stderr, "error: the code segment could not be allocated\n");
        break;
    case NO_PROG_ERROR:
        fprintf(stderr, "error: no instructions found\n");
        break;
    default:
        fprintf(stderr, "error: %d errors detected, code not loaded\n", nerr);
        break;
    }
}

/**
 * Loads the program into the VM code segment by reading instructions from a
 * file.
 * @param input The input file which contains instructions.
 * @return 0 upon success, -1 if a parsing error occurs, -2 if a memory error
 *         occurs or a strictly positive value indicating how many semantic
 *         errors were detected.
 */
int loadprog(FILE *input) {
    List *l;
    int i;
    int status = 0;
    
    labels = new_array();
    if (!labels) {
        return LABELS_ERROR;
    }
    yyin = input;
    if (yyparse()) {
        status = PARSE_ERROR;
        goto end;
    }
    if (nerr) {
        status = nerr;
        goto end;
    }
    if (prog_length == 0) {
        status = NO_PROG_ERROR;
        goto end;
    }
    /* The code segment (from vm.c). */
    prog = calloc(prog_length, sizeof(*prog));
    if (!prog) {
        perror("calloc");
        status = CODE_SEG_ERROR;
        goto end;
    }
    /*
     * Insertions into the list are done by prepending. It means that
     * instructions are stored in the list in the reverse order, i.e. the last
     * instruction of the program appears at the beginning of the list and the
     * first instruction at the end of the list.
     *
     * The list traversal is done normally but the code is loaded into the code
     * segment in the reverse order.
     */
    for (l = list, i = prog_length; list != NULL; /* No increment. */) {
        if (list->instruction.has_arg) {
            switch (list->instruction.opcode) {
            case VM_CALL: case VM_JUMP: case VM_JUMPF:
                prog[--i] = get_value_at_index(labels, list->instruction.arg);
                break;
            default:
                prog[--i] = list->instruction.arg;
                break;
            }
        }
        prog[--i] = list->instruction.opcode;
        /* Classic list memory release. */
        l = list;
        list = list->next;
        free(l);
    }
end:
    free_array(labels);
    return status;
}
