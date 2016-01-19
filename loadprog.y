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
 *  Authors: S. Lombardy, N. Bedon, C. Morvan, G. Fuhs, W. Hay, Q. Campos,
 *           J. Mangue, C. No�l.
 *
 *************************************************************************** */

%{
#include <stdio.h>
#include <stdlib.h>
#include "opcode.h"
#include "array.h"

/* Loadprog() error codes. */
#define SYNTAX_ERROR  -1
#define NOMEM_ERROR   -2
#define NOPROG_ERROR  -3
#define NOLABEL_ERROR -4

/* Flex and Bison declarations. */
int yylval;
int yyerror(const char *);
extern FILE *yyin;
extern int yylineno;
extern int yylex(void);
extern int yylex_destroy(void);

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
static Array *labels = NULL;

/** The code segment. */
extern int *prog;

/** The length of the program (excluding labels). */
int prog_length = 0;

/** The list of instructions. */
static List *instructions = NULL;

/** The number of lexical/syntactic/semantic errors. */
int nerr = 0;

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

input : input line {}
      | /* empty */ {}

line : EOL {}
     | COM1 EOL {
    	 List *tmp = new_list(instructions);
         if (!tmp) {
             return 2;
         }

         instructions = tmp;
         instructions->instruction.opcode = $1;
         instructions->instruction.has_arg = 0;
         prog_length++;
     }
     | COM2 NUM EOL {
    	 List *tmp;
         if ($1 != VM_SET && $2 < 0) {
             nerr++;
             fprintf(stderr, "line %d: %s expects an unsigned integer as "
                "argument but is here used with %d\n",
                yylineno, opcode_to_string($1), $2);
         }
         tmp = new_list(instructions);
         if (!tmp) {
             return 2;
         }
         instructions = tmp;
         instructions->instruction.opcode = $1;
         instructions->instruction.arg = $2;
         instructions->instruction.has_arg = 1;
         prog_length += 2;
     }
     | LBL NUM EOL {
         if ($2 < 0) {
             nerr++;
             fprintf(stderr, "line %d: LABEL expects an unsigned integer as "
                "argument but is here used with %d\n", yylineno, $2);
         } else {
        	 /* Check whether the label has already been declared. */
        	 if (get_value_at_index(labels, (unsigned int)$2) >= 0) {
                 fprintf(stderr, "line %d: Warning: label %d has already been declared.\n",
                    yylineno, $2);
        	 }

             /* The casts are safe since both integers are strictly positive. */
             add_value_at_index(labels, (unsigned) prog_length, (unsigned) $2);
         }
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
 * Frees a singly-linked list.
 * @param list The list to be freed.
 */
static void free_list(List *list) {
    List *l;
    while (list != NULL) {
        l = list;
        list = list->next;
        free(l);
    }
}

/**
 * Prints an error message according to a status value set by the @p loadprog
 * function or does nothing if there was no error.
 * @param errno The error code set by the @p loadprog function.
 */
static void print_load_error(int errno) {
    if (errno) {
        fprintf(stderr, "\nAbort reason: ");
    }
    switch (errno) {
    case 0: /* Success. */ break;
    case SYNTAX_ERROR:
        fprintf(stderr, "an invalid input caused a syntax error\n");
        break;
    case NOMEM_ERROR:
        fprintf(stderr, "memory exhausted\n");
        break;
    case NOPROG_ERROR:
        fprintf(stderr, "no instructions found\n");
        break;
    case NOLABEL_ERROR:
        fprintf(stderr, "jump or call to a non-existant label\n");
        break;
    default:
        fprintf(stderr, "%d lexical/syntactic/semantic ", nerr);
        fprintf(stderr, nerr > 1 ? "errors detected\n" : "error detected\n");
        break;
    }
}

/**
 * Ends the program by checking if an error occurred and freeing the memory.
 */
int endprog(int status) {
    if (status) {
        print_load_error(status);
    }
    free_list(instructions);
    free_array(labels);
    yylex_destroy();
    return status != 0;
}

/**
 * Loads the program into the VM code segment by reading instructions from a
 * file.
 * @param input The input file which contains instructions.
 * @return 0 upon success or 1 if an error occurs.
 */
int loadprog(FILE *input) {
    List *l;
    int i;
    int status = 0;
    
    labels = new_array();
    if (!labels) {
    	return endprog(NOMEM_ERROR);
    }
    yyin = input;
    status = yyparse();
    if (status == 1) {
    	return endprog(SYNTAX_ERROR);
    } else if (status == 2) {
    	return endprog(NOMEM_ERROR);
    }
    if (nerr) {
    	return endprog(nerr);
    }
    if (prog_length == 0) {
    	return endprog(NOPROG_ERROR);
    }
    /* The code segment (from vm.c). */
    prog = calloc(prog_length, sizeof(*prog));
    if (!prog) {
        perror("calloc");
    	return endprog(NOMEM_ERROR);
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
    for (l = instructions, i = prog_length; l != NULL; l = l->next) {
        if (l->instruction.has_arg) {
            switch (l->instruction.opcode) {
            case VM_CALL: case VM_JUMP: case VM_JUMPF:
                prog[--i] = get_value_at_index(labels, l->instruction.arg);
                if (prog[i] == NO_VALUE) {
                    status = NOLABEL_ERROR;
                    fprintf(stderr, "There does not exist a label %d\n",
                            l->instruction.arg);
                    /* Continue to discover other non-existent labels. */
                }
                break;
            default:
                prog[--i] = l->instruction.arg;
                break;
            }
        }
        prog[--i] = l->instruction.opcode;
    }
    
    return endprog(status);
}
