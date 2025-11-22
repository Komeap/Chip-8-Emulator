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

#ifndef DISPLAY_DISPLAY_H
#define DISPLAY_DISPLAY_H

#include <SDL2/SDL.h>
#include "sprite.h"

/**
 * \brief Emulates a 64x32 monochrome display.
 *
 * A struct Display variable shall be initialized and destroyed using the
 * Display_init and Display_destroy functions, respectively.
 *
 * This implementation relies on the SDL2 library (https://www.libsdl.org/).
 */
struct Display {

        /**
         * \brief The size at which pixels will be drawn on the physical screen.
         */
        int pixel;

        /**
         * \brief The color in which black pixels are to be drawn.
         */
        Uint32 blackcolor;

        /**
         * \brief The color in which white pixels are to be drawn.
         */
        Uint32 whitecolor;

        /**
         * \brief The internal representation of the surface (a 0-1 64*32 matrix).
         *
         * This array is dynamically allocated and freed by the Display_init
         * and Display_destroy functions, respectively.
         */
        int** contents;

        /**
         * \brief The underlying SDL window.
         */
        SDL_Window* window;

        /**
         * \brief Indicates whether the underlying SDL surface needs to be refreshed.
         */
        int modified;

        /**
         * \brief Limits the refresh rate of the display at 60 hertz.
         *
         * This flag is set every 16 ms by \p timer and then reset by \p Display_update.
         */
        SDL_atomic_t flag60;

        /**
         * \brief The underlying SDL timer.
         *
         * This timer sets \p flag60 to 1 every 16 ms.
         */
        SDL_TimerID timer;

};

/**
 * \memberof Display
 * \brief    Initializes a display.
 * \param    display The display to be initialized.
 * \param    pixel   The size at which pixels are to be drawn on the physical screen.
 * \return   0 on success, 1 in case of an error (\ref errcode is set to indicate the error).
 *
 * The \p display variable can later be passed to \ref Display_destroy to be destroyed.
 *
 * Errors:
 *   - MALLOC: out of memory,
 *   - SDL:    a SDL function has failed; call SDL_GetError() for more information.
 */
int Display_init (struct Display* display, int pixel);

/**
 * \memberof Display
 * \brief    Destroys a display.
 * \param    display The display to be destroyed.
 *
 * The \p display variable must have been initialized using the \ref Display_init function beforehand.
 */
void Display_destroy (struct Display* display);

/**
 * \memberof Display
 * \brief    Sets the colors in which black and white pixels are to be drawn.
 * \param    display The display to be modified.
 * \param    black_r The red   component of the color in which black pixels are to be drawn.
 * \param    black_g The green component of the color in which black pixels are to be drawn.
 * \param    black_b The blue  component of the color in which black pixels are to be drawn.
 * \param    white_r The red   component of the color in which white pixels are to be drawn.
 * \param    white_g The green component of the color in which white pixels are to be drawn.
 * \param    white_b The blue  component of the color in which white pixels are to be drawn.
 * \return   0 on success, 1 in case of an error (\ref errcode is set to indicate the error).
 *
 * Errors:
 *   - SDL: a SDL function has failed; call SDL_GetError() for more information.
 */
int Display_set_colors (struct Display* display, Uint8 black_r, Uint8 black_g, Uint8 black_b, Uint8 white_r, Uint8 white_g, Uint8 white_b);

/**
 * \memberof Display
 * \brief    Copies the surface of a display to the screen.
 * \param    display The display to be refreshed.
 * \return   0 on success, 1 in case of an error (\ref errcode is set to indicate the error).
 *
 * This function should be called at 60 hertz or more.
 *
 * Errors:
 *   - SDL: a SDL function has failed; call SDL_GetError() for more information.
 */
int Display_update (struct Display* display);

/**
 * \memberof Display
 * \brief    Clears a display.
 * \param    display The display to be cleared.
 * \return   0 on success, 1 in case of an error (\ref errcode is set to indicate the error).
 *
 * Errors:
 *   - SDL: a SDL function has failed; call SDL_GetError() for more information.
 */
int Display_CLS (struct Display* display);

/**
 * \memberof Display
 * \brief    Draws a sprite.
 * \param    display The display to be modified.
 * \param    sprite  The sprite to be drawn.
 * \param    x       The x-coordinate where the sprite is to be drawn.
 * \param    y       The y-coordinate where the sprite is to be drawn.
 * \param    VF      The location of the collision flag to be set (see the DRW CHIP 8 instruction for more details).
 * \return   0 on success, 1 in case of an error (\ref errcode is set to indicate the error).
 *
 * Errors:
 *   - SDL: a SDL function has failed; call SDL_GetError() for more information.
 */
int Display_DRW (struct Display* display, const struct Sprite* sprite, uint8_t x, uint8_t y, uint8_t* VF);

// endif DISPLAY_DISPLAY_H
#endif
