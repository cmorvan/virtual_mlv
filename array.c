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

#include "array.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/** The default size for a new array. */
#define DEFAULT_SIZE 128

/**
 * Allocates enough memory space for an array.
 * @return The newly allocated array.
 */
Array *new_array(void) {
    Array *array;
    
    array = malloc(sizeof(*array));
    if (!array) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    array->size = DEFAULT_SIZE;
    array->values = calloc(array->size, sizeof(*array->values));
    if (!array->values) {
        free(array);
        perror("calloc");
        exit(EXIT_FAILURE);
    }
    return array;
}

/**
 * Adds a value in the array at the given index.
 * @param array The array in which the value is to be added.
 * @param value The value to be added to the array.
 * @param index The desired position in the array for the value to be added.
 */
void add_value_at_index(Array *array, int value, int index) {
    int *tmp;
    
    if (index < 0) {
        return;
    }
    while (index > array->size) {
        array->size *= 2;
        tmp = realloc(array->values, array->size * sizeof(*array->values));
        if (!tmp) {
            free(array->values);
            perror("realloc");
            exit(EXIT_FAILURE);
        }
        array->values = tmp;
    }
    array->values[index] = value;
}

/**
 * Returns the value in the array at the given index.
 * @return The value in the array or -1 if the index was out of bounds.
 */
int get_value_at_index(const Array *array, int index) {
    return (index < 0 || index >= array->size) ? -1 : array->values[index];
}

/**
 * Frees the memory space used by an array.
 * @param array The array to be freed.
 */
void free_array(Array *array) {
    free(array->values);
    free(array);
}
