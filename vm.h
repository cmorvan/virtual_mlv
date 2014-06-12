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
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/* Maximal length of integers in base ten */
#define LENGTH_INT 10
#define MAKE_OPE_UN(n)  reg1=n; return 0;

#define MAKE_OPE_BIN(n)  reg1=n; return 0;

/* From loadprog.c */
int loadprog(char*,FILE*);




int vm_neg(void);


int vm_add(void);


int vm_sub(void);


int vm_mult(void);


int vm_div(void);


int vm_mod(void);


int vm_equal(void);


int vm_noteq(void);


int vm_low(void);

int vm_leq(void);

int vm_great(void);

int vm_geq(void);

int vm_set(int n);

int vm_load(void);

int vm_loadr(void);

int vm_save(void);

int vm_saver(void);

int vm_swap(void);

int vm_read(void);

int low_read();

int vm_write(void);

int vm_readch(void);

int vm_writech(void);

int vm_jump(int n);

int vm_jumpf(int n);

int vm_halt(void);

int vm_call(int n);

int vm_return(void);

int vm_alloc(int n);

int vm_free(int n);

int vm_push(void);

int vm_pop(void);

int vm_execute(void);
