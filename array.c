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
 *  Author: G. Fuhs, W. Hay, C. Morvan, C. Noël.
 *
 *************************************************************************** */

#include "array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** The default size for a new array. */
static const size_t DEFAULT_SIZE = 128;

/** The constant to indicate that there is no positive value in the array. */
const int NO_VALUE = -1;

/**
 * A dynamic array which cannot be declared in the stack.
 */
struct Array {
    int *values; /**< The values of the array. */
    size_t size; /**< The size of the array. */
};

static void fill(int *array, int value, size_t from, size_t to);

/**
 * Allocates enough memory space for a dynamic array.
 * @return The newly allocated dynamic array or NULL if an error occurs.
 */
Array *new_array(void) {
    Array *array;
    
    array = malloc(sizeof(*array));
    if (!array) {
        perror("malloc");
        return array;
    }
    array->size = DEFAULT_SIZE;
    array->values = malloc(array->size * sizeof(*array->values));
    if (!array->values) {
        free(array);
        perror("malloc");
        return NULL;
    }
    fill(array->values, NO_VALUE, 0, array->size);
    return array;
}

/**
 * Adds a positive value in the array at the given index.
 * @param array The array in which the value is to be added.
 * @param value The value to be added to the array.
 * @param index The desired position in the array for the value to be added.
 * @return 0 if the value was successfully added, otherwise 1.
 */
int add_value_at_index(Array *array, unsigned int value, unsigned int index) {
    int *tmp;
    size_t new_size;
    
    if (index >= array->size) {
    	new_size = array->size * 2;
    	if (new_size < index) {
    		new_size = index;
    	}

    	if (!(tmp = realloc(array->values, new_size * sizeof(*array->values)))) {
    		perror("realloc");
    		return 1;
    	}

    	array->values = tmp;
        fill(array->values, NO_VALUE, array->size, new_size);
    	array->size = new_size;
    }

    array->values[index] = value;
    return 0;
}

/**
 * Returns the value in the array at the given index.
 * @return The value in the array or NO_VALUE if the index was out of bounds.
 */
int get_value_at_index(const Array *array, unsigned int index) {
    return (index >= array->size) ? NO_VALUE : array->values[index];
}

/**
 * Frees the memory space used by an array.
 * @param array The array to be freed.
 */
void free_array(Array *array) {
    free(array->values);
    free(array);
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
 */
static void fill(int *array, int value, size_t from, size_t to) {
    size_t i;
    for (i = from; i < to; i++) {
        array[i] = value;
    }
}
