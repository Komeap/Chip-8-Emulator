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

#ifndef DISPLAY_SPRITE_H
#define DISPLAY_SPRITE_H

#include "misc/error.h"

/**
 * \brief Implements a CHIP 8 sprite.
 *
 * A sprite is a vector of bytes representing a picture.
 *
 * A struct Sprite variable shall be initialized and destroyed using the
 * Sprite_init and Sprite_destroy functions, respectively.
 */
struct Sprite {

        /**
         * \brief The length of \p contents.
         */
        size_t length;

        /**
         * \brief The capacity of \p contents.
         */
        size_t cap;

        /**
         * \brief The underlying vector of bytes.
         *
         * This array will be allocated/reallocated on demand by the Sprite_init
         * and Sprite_add functions, and finally freed by the Sprite_destroy function.
         */
        uint8_t* contents;

};

/**
 * \memberof Sprite
 * \brief    Initializes an empty sprite.
 * \param    sprite   The sprite to be initialized.
 * \param    capacity The initial capacity of the underlying vector.
 * \return   0 on success, 1 in case of an error (\ref errcode is set to indicate the error).
 *
 * The \p sprite variable can later be passed to \ref Sprite_destroy to be destroyed.
 *
 * Errors:
 *   - MALLOC: out of memory.
 */
int Sprite_init (struct Sprite* sprite, size_t capacity);

/**
 * \memberof Sprite
 * \brief    Destroys a sprite.
 * \param    sprite The sprite to be destroyed.
 *
 * The \p sprite variable must have been initialized using the \ref Sprite_init function beforehand.
 */
void Sprite_destroy (struct Sprite* sprite);

/**
 * \memberof Sprite
 * \brief    Returns the length of a sprite.
 * \param    sprite The input sprite.
 * \return   The number of bytes in \p sprite.
 */
size_t Sprite_length (const struct Sprite* sprite);

/**
 * \memberof Sprite
 * \brief    Reads the byte at a given position in a sprite.
 * \param    sprite The input sprite.
 * \param    i      The position in \p sprite of the byte to be read.
 * \param    value  The location where the byte is to be stored.
 * \return   0 on success, 1 in case of an error (\ref errcode is set to indicate the error).
 *
 * Errors:
 *   - RANGE: out of range (\p i is not in [O..length[).
 */
int Sprite_get (const struct Sprite* sprite, size_t i, uint8_t* value);

/**
 * \memberof Sprite
 * \brief    Adds a new byte at the end of a sprite.
 * \param    sprite The sprite to be modified.
 * \param    value  The byte to be added.
 * \return   0 on success, 1 in case of an error (\ref errcode is set to indicate the error).
 *
 * Errors:
 *   - MALLOC: out of memory.
 */
int Sprite_add (struct Sprite* sprite, uint8_t value);

/**
 * \memberof Sprite
 * \brief    Removes all bytes from a sprite.
 * \param    sprite The input sprite.
 *
 * The capacity of \p sprite remains the same.
 */
void Sprite_clear (struct Sprite* sprite);

// endif DISPLAY_SPRITE_H
#endif
