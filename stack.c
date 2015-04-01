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

#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int base;

/**
 * A simple stack structure.
 */
typedef struct Stack {
    int *values; /**< The values held by the stack. */
    int size; /**< The current number of values. */
    int capacity; /**< The number of values that can be held by the stack. */
} Stack;

static Stack *stack;

static const int INITIAL_CAPACITY = 128;

static int new_stack(void);
static int double_capacity(void);
static int fill(int *, int, int, int);
static int stack_addr_is_valid(int);

/**
 * Creates a new stack.
 * @return 0 upon success or 1 if an error occurs.
 */
static int new_stack(void) {
    stack = malloc(sizeof(*stack));
    if (!stack) {
        perror("malloc");
        return 1;
    }
    stack->size = 0;
    stack->capacity = INITIAL_CAPACITY;
    stack->values = calloc(stack->capacity, sizeof(*stack->values));
    if (!stack->values) {
        free(stack);
        perror("calloc");
        return 1;
    }
    return 0;
}

/**
 * Doubles the capacity of the stack.
 * @return 0 upon success or 1 if reallocation failed.
 */
static int double_capacity(void) {
    int *tmp;
    
    stack->capacity *= 2;
    tmp = realloc(stack->values, stack->capacity * sizeof(*stack->values));
    if (!tmp) {
        free(stack->values);
        perror("realloc");
        return 1;
    }
    stack->values = tmp;
    fill(stack->values, 0, stack->size, stack->capacity);
    return 0;
}

/**
 * Indicates whether a stack address is valid or not.
 * @param stack_addr The stack address whose validity has to be checked.
 * @return 1 if the stack address is valid, otherwise 0.
 */
static int stack_addr_is_valid(int stack_addr) {
    return stack != NULL && stack_addr >= 0 && stack_addr < stack->size;
}

/**
 * Frees the stack.
 * @return Always 0.
 */
int free_stack(void) {
    /* Freeing a NULL pointer is perfectly fine, nothing happens. */
    free(stack ? stack->values : NULL);
    free(stack);
    return 0;
}

/**
 * Pushes a value onto the stack.
 * @param value The value to be pushed onto the stack.
 * @return 0 upon success or 1 if an memory error occurs.
 */
int push(int value) {
    if (!stack && new_stack()) {
        return 1;
    }
    if (stack->size == stack->capacity && double_capacity()) {
        return 1;
    }
    stack->values[stack->size++] = value;
    return 0;
}

/**
 * Pops a value from the stack.
 * @param to The pointer to the location (NULL if the value is to be discarded).
 * @return 0 upon success or 1 if the stack is empty.
 */
int pop(int *to) {
    if (stack->size <= 0) {
        return 1;
    }
    stack->size--;
    if (to) {
        *to = stack->values[stack->size];
    }
    return 0;
}

/**
 * Displays the stack.
 * @return Always 0.
 */
int display_stack(void) {
    int i;
    for (i = stack->size - 1; i >= 0; i--) {
        printf("%d\n", stack->values[i]);
        if (i == base) {
            printf("____\n");
        }
    }
    return 0;
}

/**
 * Returns the size of the stack.
 * @return The size of the stack.
 */
int stack_size(void) {
    return stack->size;
}

/**
 * Loads a value from the stack.
 * @param stack_addr The stack address of the value to be loaded.
 * @param into The destination of the value loaded from the stack.
 * @return 0 upon success or 1 if the stack or @p into are @p NULL or if the
 *         stack address is not valid.
 */
int dirload(int stack_addr, int *into) {
    if (!stack || !into || !stack_addr_is_valid(stack_addr)) {
        return 1;
    }
    *into = stack->values[stack_addr];
    return 0;
}

/**
 * Saves a value into the stack.
 * @param value The value to be saved into the stack.
 * @param stack_addr The stack address of the value to be saved.
 * @return 0 upon success or 1 if the stack is @p NULL or the stack address is
 *         invalid.
 */
int dirsave(int stack_addr, int value) {
    if (!stack || !stack_addr_is_valid(stack_addr)) {
        return 1;
    }
    stack->values[stack_addr] = value;
    return 0;
}

/**
 * Fills an array with a specified value from a start index, inclusive, to an
 * end index, exclusive.
 * @param array The array to be filled.
 * @param value The value to be stored in all elements of the array.
 * @param from The index of the first element (inclusive) to be filled with the
 *             specified value.
 * @param to The index of the last element (exclusive) to be filled with the
 *           specified value.
 * @return 0 upon success otherwise 1 if the indices are not valid.
 */
static int fill(int *array, int value, int from, int to) {
    int i;
    
    if (from > to || from < 0 || to > stack->capacity) {
        return 1;
    }
    for (i = from; i < to; i++) {
        array[i] = value;
    }
    return 0;
}
