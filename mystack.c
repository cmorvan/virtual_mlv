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

#include <stdio.h>
#include <stdlib.h>
#define STACK_SIZE 128

typedef struct ST{
    int v_stack[STACK_SIZE];
    struct ST* Snext;
} stack;

static int occstack = STACK_SIZE;
static int stack_depth = -1;
static stack * the_stack;

extern int base;

int create_new_stack(void) {
    stack *s;
    
    s = malloc(sizeof(*s));
    if (!s) {
        perror("malloc");
        return 1;
    }
    s->Snext = the_stack;
    occstack = 0;
    the_stack = s;
    stack_depth++;
    return 0;
}

int delete_a_stack(void) {
    stack *s;
    
    if (the_stack == NULL) {
        return 1;
    }
    s = the_stack->Snext;
    free(the_stack);
    the_stack = s;
    stack_depth--;
    /* Correcting deletion bug */
    occstack = STACK_SIZE;
    return 0;
}

int free_stack(void) {
    while (delete_a_stack() == 0);
    return 0;
}

int push(int val) {
    if (occstack == STACK_SIZE) {
        if (create_new_stack()) {
            return 1;
        }
    }
    the_stack->v_stack[occstack++] = val;
    return 0;
}

int pop(int *val) {
    if (occstack == 0) {
        if (delete_a_stack()) {
            return 1;
        }
    }
    if (val != NULL)
    *val = the_stack->v_stack[occstack - 1];
    occstack--;
    return 0;
}

int dirload(int i, int *val) {
    stack *s = the_stack;
    int st = stack_depth;
    while (s != NULL && st * STACK_SIZE > i) {
        s = s->Snext;
        st--;
    }
    if (s == NULL) {
        return 1;
    }
    if (st == stack_depth && i % STACK_SIZE >= occstack) {
        return 1;
    }
    *val = s->v_stack[i % STACK_SIZE];
    return 0;
}

int dirsave(int i, int val) {
    stack *s = the_stack;
    int st = stack_depth;
    while (s != NULL && st * STACK_SIZE > i) {
        s = s->Snext;
        st--;
    }
    if (s == NULL) {
        return 1;
    }
    if (st == stack_depth && i % STACK_SIZE >= occstack) {
        return 1;
    }
    s->v_stack[i % STACK_SIZE] = val;
    return 0;
}

int display_stack(void) {
    stack *s = the_stack;
    int i = occstack - 1;
    int st = stack_depth;
    while (s != NULL) {
        for (; i >= 0; i--) {
            printf("%d\n", s->v_stack[i]);
            if (base == st * STACK_SIZE + i) {
                printf("____\n");
            }
        }
        i = STACK_SIZE - 1;
        s = s->Snext;
        st--;
    }
    return 0;
}

int stack_size(void) {
    return occstack + stack_depth * STACK_SIZE;
}
