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
 *	dynArray.c
 * 
 * This file contains functions enabeling to allocate strictly
 * positive integers into a dynamically allocated array.
 *
 * newDynArray: creation
 * addValue: write
 * getValue: retrieve -- -1 when there is some error
 * freeArray: free allocated space.
 * 
 *************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "dynArray.h"

dynArray newDynArray(){
  dynArray newArray;
  newArray.dynArray=NULL;
  newArray.size=0;
  return newArray;
}

void addValue(dynArray * array, int key ,int value )
{
	int *tabTmp;
	if(array->size==0)
	{
		if( (array->dynArray=calloc(128,sizeof(int)))==NULL)
		{
			perror("calloc");
			exit(EXIT_FAILURE);
		}
		array->size=128;
	}
	while(key>(array->size))
	{
		if((tabTmp=realloc(array->dynArray,sizeof(int)*array->size*2))== NULL)
		{
			perror("realloc");
			exit(EXIT_FAILURE);
		}
		array->dynArray=tabTmp;
		array->size*=2;
	}
	array->dynArray[key]=value;
}

int getValue(dynArray * array,int key)
{
  return (key < 0 || key >= array->size) ? -1 : array->dynArray[key];
}

void freeArray(dynArray * array)
{
	free(array->dynArray);
	array->dynArray=NULL;
	array->size=0;
}
