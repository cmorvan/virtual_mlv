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

#ifndef OPCODE_H
#define OPCODE_H

#include <limits.h>

/**
 * Opcodes are now first-class type rather than macros.
 */
typedef enum Opcode {
    /* Without argument. */
    VM_NOP,
    VM_NEG,
    VM_ADD,
    VM_SUB,
    VM_MUL,
    VM_DIV,
    VM_MOD,
    VM_EQUAL,
    VM_NOTEQ,
    VM_LESS,
    VM_LEQ,
    VM_GREATER,
    VM_GEQ,
    VM_PUSH,
    VM_POP,
    VM_SWAP,
    VM_READ,
    VM_WRITE,
    VM_HALT,
    VM_RETURN,
    VM_LOAD,
    VM_LOADR,
    VM_SAVE,
    VM_SAVER,
    VM_READCH,
    VM_WRITECH,
    VM_TOPST,
    
    /* With one argument. */
    VM_SET,
    VM_JUMP,
    VM_JUMPF,
    VM_LABEL,
    VM_CALL,
    VM_ALLOC,
    VM_FREE,
    
    /*
     * The sole purpose of the following opcode is to maintain backward
     * compatibility with old code. Former opcodes were defined using macros
     * which expanded to an `int'. Our new opcode enum starts by 0 and the
     * absence of negative value could have lead the compiler to choose an
     * unsigned integral type and/or another integral with a smaller width.
     */
    __VM_RESERVED = INT_MIN
} Opcode;

const char *opcode_to_string(Opcode);
void print_code_segment(const int *, int);

#endif /* OPCODE_H */
