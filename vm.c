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


#include "mystack.h"
#include "vm.h"
#include <ctype.h>
#include <errno.h>
#include <limits.h>

/** The first register. */
static int reg1;

/** The second register. */
static int reg2;

int base = 0;

/** The code itself. */
Opcode *prog = NULL;

/** The program counter. */
int prog_counter = 0;

/** The debug flag. */
int debug = 0;

static int vm_nop(void);
static int vm_neg(void);
static int vm_add(void);
static int vm_sub(void);
static int vm_mul(void);
static int vm_div(void);
static int vm_mod(void);
static int vm_equal(void);
static int vm_noteq(void);
static int vm_less(void);
static int vm_leq(void);
static int vm_greater(void);
static int vm_geq(void);
static int vm_set(int);
static int vm_load(void);
static int vm_loadr(void);
static int vm_save(void);
static int vm_saver(void);
static int vm_swap(void);
static int vm_read(void);
static int read_int(void);
static int vm_write(void);
static int vm_readch(void);
static int vm_writech(void);
static int vm_jump(int);
static int vm_jumpf(int);
static int vm_call(int);
static int vm_return(void);
static int vm_alloc(int);
static int vm_free(int);
static int vm_push(void);
static int vm_pop(void);

/**
 * Does nothing at all.
 * @return Always 0.
 */
static int vm_nop(void) {
    return 0;
}

/**
 * Changes the sign of @p reg1.
 * @return Always 0.
 */
static int vm_neg(void) {
    reg1 = -reg1;
    return 0;
}

/**
 * Adds @p reg1 to @p reg2 and stores the result into @p reg1.
 * @return Always 0.
 */
static int vm_add(void) {
    reg1 = reg1 + reg2;
    return 0;
}

/**
 * Subtracts @p reg2 from @p reg1 and stores the result into @p reg1.
 * @return Always 0.
 */
static int vm_sub(void) {
    reg1 = reg1 - reg2;
    return 0;
}

/**
 * Multiplies @p reg1 by @p reg2 and stores the result into @p reg1.
 * @return Always 0.
 */
static int vm_mul(void) {
    reg1 = reg1 * reg2;
    return 0;
}

/**
 * Divides @p reg1 by @p reg2 and stores the result into @p reg1.
 * @return 0 upon success or 1 if it was a division by 0.
 */
static int vm_div(void) {
    if (reg2 == 0) {
        fprintf(stderr, "Floating point exception: division by 0\n");
        return 1;
    }
    reg1 = reg1 / reg2;
    return 0;
}

/**
 * Computes the remainder of @p reg1 divided by @p reg2.
 * @return 0 upon success or 1 if it was a remainder by 0.
 */
static int vm_mod(void) {
    if (reg2 == 0) {
        fprintf(stderr, "Floating point exception: remainder by 0\n");
        return 1;
    }
    reg1 = reg1 % reg2;
    return 0;
}

/**
 * Tests if @p reg1 is equal to @p reg2 and stores the result into @p reg1.
 * @return Always 0.
 */
static int vm_equal(void) {
    reg1 = (reg1 == reg2);
    return 0;
}

/**
 * Tests if @p reg1 is not equal to @p reg2 and stores the result into @p reg1.
 * @return Always 0.
 */
static int vm_noteq(void) {
    reg1 = (reg1 != reg2);
    return 0;
}

/**
 * Tests if @p reg1 is less than @p reg2 and stores the result into @p reg1.
 * @return Always 0.
 */
static int vm_less(void) {
    reg1 = (reg1 < reg2);
    return 0;
}

/**
 * Tests if @p reg1 is less than or equal to @p reg2 and stores the result
 * into @p reg1.
 * @return Always 0.
 */
static int vm_leq(void) {
    reg1 = (reg1 <= reg2);
    return 0;
}

/**
 * Tests if @p reg1 is greater than @p reg2 and stores the result into @p reg1.
 * @return Always 0.
 */
static int vm_greater(void) {
    reg1 = (reg1 > reg2);
    return 0;
}

/**
 * Tests if @p reg1 is greater than or equal to @p reg2 and stores the result
 * into @p reg1.
 * @return Always 0.
 */
static int vm_geq(void) {
    reg1 = (reg1 >= reg2);
    return 0;
}

/**
 * Sets the value of @p reg1.
 * @param n The value to be set to @p reg1.
 * @return Always 0.
 */
static int vm_set(int n) {
    reg1 = n;
    return 0;
}

/******************************************************************************/
static int vm_load(void) {
    if (dirload(reg1, &reg1)) {
        fprintf(stderr, "LOAD: illegal stack access\n");
        return 1;
    }
    return 0;
}

static int vm_loadr(void) {
    if (dirload(reg1 + base, &reg1)) {
        fprintf(stderr, "LOADR: illegal stack access\n");
        return 1;
    }
    return 0;
}

static int vm_save(void) {
    if (dirsave(reg2, reg1)) {
        fprintf(stderr, "SAVE: illegal stack access\n");
        return 1;
    }
    return 0;
}   

static int vm_saver(void) {
    if (dirsave(reg2 + base, reg1)) {
        fprintf(stderr, "SAVER: illegal stack access\n");
        return 1;
    }
    return 0;
}
/******************************************************************************/

/**
 * Swaps values of @p reg1 and @p reg2.
 * @return Always 0.
 */
static int vm_swap(void) {
    reg1 ^= reg2;
    reg2 ^= reg1;
    reg1 ^= reg2;
    return 0;
}

/**
 * Reads a number from the standard input and stores it into @p reg1.
 * @return Always 0.
 */
static int vm_read(void) {
    printf("<< ");
    reg1 = read_int();
    return 0;
}

/**
 * Reads an integer from the standard input and returns it.
 * @return The integer read from the standard input.
 */
static int read_int(void) {
    long val;
    char *endptr;
    char str[BUFSIZ] = {0};
    
    fgets(str, sizeof(str), stdin);
    /* To distinguish success/failure after call. */
    errno = 0;
    val = strtol(str, &endptr, 10);
    /* Check for various possible errors, see strtol(3) for details. */
    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
            || (errno != 0 && val == 0)) {
        perror("strtol");
        exit(EXIT_FAILURE);
    }
    if (endptr == str) {
        fprintf(stderr, "Number format error: no digits were found\n");
        exit(EXIT_FAILURE);
    }
    /* Check for possible overflow because the VM only uses int. */
    if (val < INT_MIN || val > INT_MAX) {
        fprintf(stderr, "Number format error: result out of an `int' range\n");
        exit(EXIT_FAILURE);
    }
    /* This is not necessarily an error, but we do not want to accept anything
     after the number even if it is legal for strtol. */
    if (*endptr) {
        fprintf(stderr, "Number format error: trailing characters detected\n");
        exit(EXIT_FAILURE);
    }
    return (int) val;
}

/**
 * Displays the value of @p reg1 to the standard output.
 * @return Always 0.
 */
static int vm_write(void) {
    printf(">> %d\n", reg1);
    return 0;
}

/**
 * Reads a character from the standard input and stores its value into @p reg1.
 * @return Always 0.
 */
static int vm_readch(void) {
    reg1 = getchar();
    return 0;
}

/**
 * Writes the value of @p reg1 to the standard output as a character.
 * @return Always 0.
 */
static int vm_writech(void) {
    putchar(reg1);
    return 0;
}

/**
 * Unconditionally jumps to a label.
 * @param n The label to jump to.
 * @return 0 upon success or 1 if the label is out of bounds.
 */
static int vm_jump(int n) {
    if (n < 0 || n >= prog_length) {
        fprintf(stderr, "JUMP: jump label is out of bounds\n");
        return 1;
    }
    prog_counter = n;
    return 0;
}

/**
 * Jumps to a label if and only if @p reg1 is false (i.e. equal to 0).
 * @param n The label to jump to.
 * @return 0 upon success or 1 if the label is out of bounds.
 */
static int vm_jumpf(int n) {
    if (reg1 == 0) {
        if (n < 0 || n >= prog_length) {
            fprintf(stderr, "JUMPF: illegal jump\n");
            return 1;
        }
        prog_counter = n;
    }
    return 0;
}

/**
 *
 */
static int vm_call(int n) {
    if (push(prog_counter + 1)) {
        fprintf(stderr, "Stack overflow\n");
        return 1;
    }
    if (push(base)) {
        fprintf(stderr, "Stack overflow\n");
        return 1;
    }
    base = stack_size();
    return vm_jump(n);
}

/**
 *
 */
static int vm_return(void) {
    while (stack_size() > base) {
        if (pop(NULL)) {
            fprintf(stderr, "RETURN: stack empty\n");
            return 1;
        }
    }
    pop(&base);
    pop(&prog_counter);
    return 0;
}

/**
 * Allocates memory in the stack.
 * @param n The size of the memory allocation.
 * @return 0 upon success or 1 if a stack overflow occurs.
 */
static int vm_alloc(int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (push(0)) {
            fprintf(stderr, "ALLOC: stack overflow\n");
            return 1;
        }
    }
    return 0;
}

/**
 * Frees memory allocated in the stack.
 * @param n The size of the memory deallocation.
 * @return 0 upon success or 1 if the stack is empty.
 */
static int vm_free(int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (pop(NULL)) {
            fprintf(stderr, "FREE: pile vide\n");
            return 1;
        }
    }
    return 0;
}

/**
 * Pushes the value of @p reg1 onto the stack.
 * @return 0 upon success or 1 if a stack overflow occurs.
 */
static int vm_push(void) {
    if (push(reg1)) {
        fprintf(stderr, "PUSH: Stack overflow\n");
        return 1;
    }
    return 0;
}

/**
 * Pops a value from the stack and stores it into @p reg1.
 * @return 0 upon success or 1 if the stack is empty.
 */
static int vm_pop(void) {
    if (pop(&reg1)) {
        fprintf(stderr, "FREE: Stack empty\n");
        return 1;
    }
    return 0;
}

/**
 * Halts the VM.
 * @return Always 0.
 */
int vm_halt(void) {
    free(prog);
    free_stack();
    return 0;
}

/**
 * Executes the code.
 * @return 0 upon success or 1 if the program contains illegal opcodes.
 */
int vm_execute(void) {
    int halt = 0;
    while (!halt) {
        switch (prog[prog_counter++]) {
        case VM_NOP:        vm_nop();       break;
        case VM_NEG:        vm_neg();       break;
        case VM_ADD:        vm_add();       break;
        case VM_SUB:        vm_sub();       break;
        case VM_MUL:        vm_mul();       break;
        case VM_DIV:        vm_div();       break;
        case VM_MOD:        vm_mod();       break;
        case VM_EQUAL:      vm_equal();     break;
        case VM_NOTEQ:      vm_noteq();     break;
        case VM_LESS:       vm_less();      break;
        case VM_LEQ:        vm_leq();       break;
        case VM_GREATER:    vm_greater();   break;
        case VM_GEQ:        vm_geq();       break;
        case VM_SET:        vm_set(prog[prog_counter++]); break;
        case VM_LOAD:       vm_load();      break;
        case VM_LOADR:      vm_loadr();     break;
        case VM_SAVE:       vm_save();      break;
        case VM_SAVER:      vm_saver();     break;
        case VM_JUMP:       vm_jump(prog[prog_counter]); break;
        case VM_JUMPF:      vm_jumpf(prog[prog_counter++]); break;
        case VM_PUSH:       vm_push();      break;
        case VM_POP:        vm_pop();       break;
        case VM_SWAP:       vm_swap();      break;
        case VM_READ:       vm_read();      break;
        case VM_WRITE:      vm_write();     break;
        case VM_READCH:     vm_readch();    break;
        case VM_WRITECH:    vm_writech();   break;
        case VM_HALT:       halt = 1;       break;
        case VM_CALL:       vm_call(prog[prog_counter]); break;
        case VM_RETURN:     vm_return();    break;
        case VM_ALLOC:      vm_alloc(prog[prog_counter++]); break;
        case VM_FREE:       vm_free(prog[prog_counter++]); break;
                
        /* These values cannot be loaded into our program nor be really matched.
         However, they need to remain within the switch for two reasons.
         Firstly because the compiler does not accept unmatched enum values and
         secondly because once these two values are matched, the compiler can
         warn us about real opcodes that are left unmatched. */
        case VM_LABEL: case __VM_RESERVED:
            fprintf(stderr, "Fatal error: illegal opcode encountered\n");
            return 1;
        /* No default case to let the compiler ensure that all opcodes values
         are matched. */
        }
        if (debug) {
            display_stack();
            printf("prog line: %d  register 1: %d   register 2: %d\n",
                   prog_counter, reg1, reg2);
            getchar();
        }
    }
    return 0;
}
