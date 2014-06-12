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
###  Authors: S. Lombardy, N. Bedon, C. Morvan, G. Fuhs
###
### This makefile is intended for amd64 machines.
### Otherwise only 686 binaries may be generated.
###
### all -> produces amd64 binary
### all32 -> produces 32 bits (686) binary
###

CC=gcc
FLAGS32=-m32
CFLAGS=-Wall
EXEC=vm
EXEC64=vmAMD64
EXEC32=vm686
EXLOAD=loadprog
LOC_LIBS=dynArray

all: $(EXEC64) clean

all32: $(EXEC32) clean

$(EXEC64): $(EXEC).o $(EXLOAD).o $(LOC_LIBS).o  lex.yy.o mystack.o
	gcc  -o $@ $^ $(CFLAGS)

$(EXEC32): $(EXEC).o32 $(EXLOAD).o32 $(LOC_LIBS).o32  lex.yy.o32 mystack.o32
	gcc   $(FLAGS32) -o $@ $^ $(CFLAGS)

$(EXLOAD).c: $(EXLOAD).y
	bison -d -o $(EXLOAD).c $(EXLOAD).y

$(EXLOAD).h: $(EXLOAD).c

lex.yy.c: $(EXLOAD).lex $(EXLOAD).h
	flex $(EXLOAD).lex

%.o: %.c
	gcc -o $@ -c $< $(CFLAGS)

%.o32: %.c
	gcc $(FLAGS32) -o $@ -c $< $(CFLAGS)

clean:
	rm -f *.o *.o32 lex.yy.c $(EXLOAD).[ch]