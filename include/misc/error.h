/*-------------------------------------------------------------------------*
 | Copyright (C) 2023 Département Informatique de PolyTech Tours.          |
 |                                                                         |
 | This file is part of PolyChip8, yet another CHIP 8 emulator.            |
 |                                                                         |
 | PolyChip8 is free software; you can redistribute it and/or modify       |
 | it under the terms of the GNU General Public License as published by    |
 | the Free Software Foundation; either version 3 of the License,          |
 | or (at your option) any later version.                                  |
 |                                                                         |
 | PolyChip8 is distributed in the hope that it will be useful,            |
 | but WITHOUT ANY WARRANTY; without even the implied warranty of          |
 | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            |
 | GNU General Public License for more details.                            |
 |                                                                         |
 | You should have received a copy of the GNU General Public License       |
 | along with this program. If not, see <http://www.gnu.org/licenses/>.    |
 *-------------------------------------------------------------------------*/

#ifndef MISC_TYPES_H
#define MISC_TYPES_H

#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

/**
 * \brief A code describing the last error encountered in the program.
 *
 * Possible values are:
 *   - QUIT:     quit event;
 *   - MALLOC:   out of memory;
 *   - SEGFAULT: segmentation fault;
 *   - RANGE:    an index is out of range;
 *   - SDL:      a SDL function has failed; call SDL_GetError() for more information;
 *   - STACK:    invalid operation on the call stack;
 *   - OPCODE:   unknown instruction;
 *   - PERM:     the file does not exist or cannot be read;
 *   - BADARG:   invalid argument.
 */
extern int errcode;

/**
 * \brief  Returns a message about the last error encountered in the program.
 * \return A description of the error.
 *
 * If \ref errcode is in an inconsistent state, "UNKNOWN" is returned.
 *
 * The returned string is statically allocated and shall neither be modified, nor freed.
 */
const char* errorstr ();

#define QUIT      1
#define MALLOC    2
#define SEGFAULT  3
#define RANGE     4
#define SDL       5
#define STACK     6
#define OPCODE    7
#define PERM      8
#define BADARG    9

// ifndef MISC_TYPES_H
#endif
