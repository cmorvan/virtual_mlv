### This file is part of virtual_mlv.
###
### virtual_mlv is free software: you can redistribute it and/or modify
### it under the terms of the GNU General Public License as published
### by the Free Software Foundation, either version 3 of the License,
### or (at your option) any later version.
###
### virtual_mlv is distributed in the hope that it will be useful, but
### WITHOUT ANY WARRANTY; without even the implied warranty of
### MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
### General Public License for more details.
###
### You should have received a copy of the GNU General Public License
### along with virtual_mlv.  If not, see
### <http://www.gnu.org/licenses/>.
###
###
### Authors: S. Lombardy, N. Bedon, C. Morvan, G. Fuhs, W. Hay
###

CC = gcc
LEXER = flex
PARSER = bison
CFLAGS = -Wall -ansi -pedantic
EXEC = vm

HAS_CLANG = $(shell which clang > /dev/null 2>&1; echo $$?)
CLANG = # Used to check doxygen comments consistency.

ifeq ($(HAS_CLANG), 0) # 0 is fine, it's the shell exit code upon success.
    CLANG = clang -Wdocumentation -fsyntax-only *.c *.h
endif

all: $(EXEC) clean

$(EXEC): main.o vm.o loadprog.o array.o lex.yy.o mystack.o
	$(CC) $(CFLAGS) -o $@ $^

loadprog.c: loadprog.y
	$(PARSER) -d -o loadprog.c loadprog.y

loadprog.h: loadprog.c

lex.yy.c: loadprog.lex loadprog.h
	$(LEXER) loadprog.lex

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f *.o lex.yy.c loadprog.[ch]