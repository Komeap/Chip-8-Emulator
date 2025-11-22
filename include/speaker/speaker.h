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

#ifndef SPEAKER_SPEAKER_H
#define SPEAKER_SPEAKER_H

#include <SDL2/SDL.h>
#include "misc/error.h"

/**
 * \brief Emulates a monotone speaker (buzzer).
 *
 * This relies on the SDL2 library (https://www.libsdl.org/) and is a
 * simplification of danirod's implementation (https://github.com/danirod/chip8).
 */
struct Speaker {

        /**
         * \brief The underlying SDL audio device.
         */
        SDL_AudioDeviceID device;

};

/**
 * \memberof Speaker
 * \brief    Initializes a speaker.
 * \param    speaker The speaker to be initialized.
 * \return   0 on success, 1 in case of an error (\ref errcode is set to indicate the error).
 *
 * The \p speaker variable can later be passed to \ref Speaker_destroy to be destroyed.
 *
 * Errors:
 *   - SDL: a SDL function has failed; call SDL_GetError() for more information.
 */
int Speaker_init (struct Speaker* speaker);

/**
 * \memberof Speaker
 * \brief    Destroys a speaker.
 * \param    speaker The speaker to be destroyed.
 *
 * The \p speaker variable must have been initialized using the \ref Speaker_init function beforehand.
 */
void Speaker_destroy (struct Speaker* speaker);

/**
 * \memberof Speaker
 * \brief    Turns on a speaker.
 * \param    speaker The speaker to be turned on.
 */
void Speaker_on (struct Speaker* speaker);

/**
 * \memberof Speaker
 * \brief    Turns off a speaker.
 * \param    speaker The speaker to be turned off.
 */
void Speaker_off (struct Speaker* speaker);

// ifndef SPEAKER_SPEAKER_H
#endif
