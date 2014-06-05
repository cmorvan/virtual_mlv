/* ***************************************************************************
 *
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
 *  Author: G. Fuhs, W. Hay, C. Morvan
 *
 *************************************************************************** */

/* ***************************************************************************
 *	dynArray.h
 * 
 * This file contains functions enabling to allocate strictly
 * positive integers into a dynamically allocated array.
 *
 * newDynArray: creation
 * addValue: write
 * getValue: retrieve -- -1 when there is some error
 * freeArray: free allocated space.
 * 
 *************************************************************************** */

#ifndef DYNARRAY_H
#define DYNARRAY_H

typedef struct struct_dynArray{
  int* dynArray;
  int size;
}dynArray;

dynArray newDynArray();
void addValue(dynArray * , int key ,int value );
int getValue(dynArray * , int);
void freeArray(dynArray *);

#endif
