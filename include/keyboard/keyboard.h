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

#ifndef INPUT_KEYBOARD_H
#define INPUT_KEYBOARD_H

#include <SDL2/SDL.h>
#include "misc/error.h"

/**
 * \brief Emulates a 16-key keypad.
 *
 * A struct Keyboard variable shall be initialized and destroyed using the
 * Keyboard_init and Keyboard_destroy functions, respectively.
 *
 * This implementation relies on the SDL2 library (https://www.libsdl.org/).
 */
struct Keyboard {

        /**
         * \brief The SDL keyboard scancode associated with each key (16 keys).
         *
         * This array is dynamically allocated and freed by the Keyboard_init
         * and Keyboard_destroy functions, respectively.
         */
        Uint8* map;

};

/**
 * \memberof Keyboard
 * \brief    Initializes a keyboard.
 * \param    keyboard The keyboard to be initialized.
 * \return   0 on success, 1 in case of an error (\ref errcode is set to indicate the error).
 *
 * The \p keyboard variable can later be passed to \ref Keyboard_destroy to be destroyed.
 *
 * Errors:
 *   - MALLOC: out of memory.
 */
int Keyboard_init (struct Keyboard* keyboard);

/**
 * \memberof Keyboard
 * \brief    Destroys a keyboard.
 * \param    keyboard The keyboard to be destroyed.
 *
 * The \p keyboard variable must have been initialized using the \ref Keyboard_init function beforehand.
 */
void Keyboard_destroy (struct Keyboard* keyboard);

/**
 * \memberof Keyboard
 * \brief    Retrieves the current state of a keyboard.
 * \param    keyboard The keyboard to be observed.
 * \param    key      The key      to be observed.
 * \param    state    The location where the state code is to be stored
 *                      (KEY_DOWN if \p key is being pressed, KEY_UP otherwise).
 * \return   0 on success, 1 in case of an error (\ref errcode is set to indicate the error).
 *
 * In case of an error, \p *state is neither accessed, nor modified.
 *
 * Errors:
 *   - QUIT:  quit event;
 *   - RANGE: out of range (\p key is not in [0..15]).
 */
int Keyboard_get (const struct Keyboard* keyboard, uint8_t key, int* state);

#define KEY_UP   0
#define KEY_DOWN 1

/**
 * \memberof Keyboard
 * \brief    Waits for any key to be pressed.
 * \param    keyboard The keyboard to be observed.
 * \param    pressed  The location where the key pressed is to be stored.
 * \return   0 on success, 1 in case of an error (\ref errcode is set to indicate the error).
 *
 * This function is blocking.
 *
 * Errors:
 *   - QUIT: quit event;
 *   - SDL:  a SDL function has failed; call SDL_GetError() for more information.
 */
int Keyboard_wait (const struct Keyboard* keyboard, uint8_t* pressed);

// ifndef INPUT_KEYBOARD_H
#endif
