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

#ifndef ARRAY_H
#define ARRAY_H

extern const int NO_VALUE;

typedef struct Array Array;

Array *new_array(void);
int add_value_at_index(Array *, unsigned, unsigned);
int get_value_at_index(const Array *, unsigned);
void free_array(Array *);

#endif /* ARRAY_H */
