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
 *  Authors: S. Lombardy, N. Bedon, C. Morvan, W. Hay
 *
 *************************************************************************** */

#include "opcode.h"
#include <stdio.h>
#include <stdlib.h>

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
    case __VM_RESERVED: return "__RESERVED";
    }
    /* This part of the code is supposed to be unreachable. */
    fprintf(stderr, "Fatal error: unmatched opcode\n");
    return "";
}
