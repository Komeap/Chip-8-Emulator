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

#ifndef MISC_DEBUG_H
#define MISC_DEBUG_H

#include <stdint.h>

/**
 * \brief  Returns a text description of an instruction (for debugging purposes).
 * \param  instruction The instruction to be decoded and described.
 * \return A text description of \p instruction.
 *
 * The returned string is statically allocated and shall neither be modified, nor freed.
 */
const char* instruction_as_str (uint16_t instruction);

// ifndef MISC_DEBUG_H
#endif
