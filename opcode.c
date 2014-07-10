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

#include "opcode.h"
#include <stdio.h>
#include <stdlib.h>

static int has_runtime_arg(Opcode);
static int is_a_jump(Opcode);

/**
 * Returns the string representation of an opcode.
 * @param opcode The opcode whose string representation is desired.
 * @return The string representation of the opcode.
 */
const char *opcode_to_string(Opcode opcode) {
    switch (opcode) {
    case VM_NOP:        return "NOP";
    case VM_NEG:        return "NEG";
    case VM_ADD:        return "ADD";
    case VM_SUB:        return "SUB";
    case VM_MUL:        return "MUL";
    case VM_DIV:        return "DIV";
    case VM_MOD:        return "MOD";
    case VM_EQUAL:      return "EQUAL";
    case VM_NOTEQ:      return "NOTEQ";
    case VM_LESS:       return "LESS";
    case VM_LEQ:        return "LEQ";
    case VM_GREATER:    return "GREATER";
    case VM_GEQ:        return "GEQ";
    case VM_SET:        return "SET";
    case VM_LOAD:       return "LOAD";
    case VM_LOADR:      return "LOADR";
    case VM_SAVE:       return "SAVE";
    case VM_SAVER:      return "SAVER";
    case VM_JUMP:       return "JUMP";
    case VM_JUMPF:      return "JUMPF";
    case VM_PUSH:       return "PUSH";
    case VM_POP:        return "POP";
    case VM_SWAP:       return "SWAP";
    case VM_READ:       return "READ";
    case VM_WRITE:      return "WRITE";
    case VM_READCH:     return "READCH";
    case VM_WRITECH:    return "WRITECH";
    case VM_HALT:       return "HALT";
    case VM_CALL:       return "CALL";
    case VM_RETURN:     return "RETURN";
    case VM_ALLOC:      return "ALLOC";
    case VM_FREE:       return "FREE";
    case VM_LABEL:      return "LABEL";
    case VM_TOPST:      return "TOPST";
    case __VM_RESERVED: return "__RESERVED";
    }
    /* This part of the code is supposed to be unreachable. */
    fprintf(stderr, "Fatal error: unmatched opcode\n");
    return "";
}

/**
 * Prints a code segment.
 * @param code The code segment to be printed.
 * @param length The length of the code segment to be printed.
 */
void print_code_segment(const int *code, int length) {
    int i;
    /*
     * A flag to indicate if the code has to be treated as an argument. It
     * allows us to distinguish opcodes from their arguments when printing the
     * code segment.
     */
    int as_arg = 0;
    /*
     * A flag to indicate if the code implies a jump. It allows us to make a
     * distinction between a jump argument which is prefixed by '#' and a normal
     * argument which isn't.
     */
    int as_jump = 0;
    
    printf(" Code:\n");
    for (i = 0; i < length; i++) {
        if (!as_arg) {
            printf("%5d: %s\n", i, opcode_to_string(code[i]));
            if (has_runtime_arg(code[i])) {
                as_arg = 1;
            }
            if (is_a_jump(code[i])) {
                as_jump = 1;
            }
        } else {
            printf(as_jump ? "%5d: #%d\n" : "%5d: %d\n", i, code[i]);
            as_arg = 0;
            as_jump = 0;
        }
    }
}

/**
 * Tells whether an opcode has an argument at runtime.
 * @param opcode The opcode to be tested.
 * @return 1 if the opcode has a runtime argument, otherwise 0.
 */
static int has_runtime_arg(Opcode opcode) {
    switch (opcode) {
    case VM_SET: case VM_JUMP: case VM_JUMPF: case VM_CALL: case VM_ALLOC:
    case VM_FREE:
        return 1;
    default:
        break;
    }
    return 0;
}

/**
 * Tells whether an opcode implies a jump.
 * @param opcode The opcode to be tested.
 * @return 1 if the opcode implies a jump, otherwise 0.
 */
static int is_a_jump(Opcode opcode) {
    switch (opcode) {
    case VM_JUMP: case VM_JUMPF: case VM_CALL:
        return 1;
    default:
        break;
    }
    return 0;
}
