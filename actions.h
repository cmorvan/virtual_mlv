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
/* 2013 : 060 et 061 : prise en compte des caracteres */
/*Sans argument*/
#define VM_NEG      00
#define VM_ADD      01
#define VM_SUB      02
#define VM_MULT     03
#define VM_DIV      04
#define VM_MOD      05
#define VM_EQUAL    010
#define VM_NOTEQ    011
#define VM_LOW      012
#define VM_LEQ      013
#define VM_GREAT    014
#define VM_GEQ      015
#define VM_PUSH     020
#define VM_POP      021
#define VM_SWAP     022
#define VM_READ     023
#define VM_WRITE    024
#define VM_HALT     050
#define VM_RETURN   051
#define VM_LOAD     030
#define VM_LOADR    031
#define VM_SAVE     032
#define VM_SAVER    034
/* New*/
#define VM_READCH    060
#define VM_WRITECH    061

/*Avec argument*/

#define VM_SET      027
#define VM_JUMP     040
#define VM_JUMPF    041
#define VM_LABEL    042
#define VM_CALL     052
#define VM_ALLOC    035
#define VM_FREE     036
