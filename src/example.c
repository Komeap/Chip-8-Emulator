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
#include <display/display.h>
/// @file Fichier example.c donner par les prof pour verifier que ca marche.
/**
 * @brief Pour l'instant, ouvre l'émulateur et fait clignoter Go!!
 */
int main() {
        struct Display display;
        if (!Display_init(&display, 10)) {
                printf("Display initialized.\n");
                if (!Display_set_colors(&display, 0, 0, 0, 0, 255, 0)) {
                        printf("Green theme selected.\n");
                        struct Sprite sprite;
                        if (!Sprite_init(&sprite, 6)) {
                                printf("Sprite initialized.\n");
                                for (int i = 0; i < 19; ++i) {
                                        uint8_t collision = 0;
                                        // draw G
                                        if (    Sprite_add(&sprite, 0b11110000)
                                             || Sprite_add(&sprite, 0b10000000)
                                             || Sprite_add(&sprite, 0b10000000)
                                             || Sprite_add(&sprite, 0b10110000)
                                             || Sprite_add(&sprite, 0b10010000)
                                             || Sprite_add(&sprite, 0b11110000)
                                        ) {
                                                fprintf(stderr, "The initialization of the sprite 'G' failed (%s).\n", errorstr());
                                        }
                                        if (Display_DRW(&display, &sprite, 24, 10, &collision)) {
                                                fprintf(stderr, "Display_DRW failed (%s).\n", errorstr());
                                        }
                                        Sprite_clear(&sprite);
                                        // draw O
                                        if (    Sprite_add(&sprite, 0b11110000)
                                             || Sprite_add(&sprite, 0b10010000)
                                             || Sprite_add(&sprite, 0b10010000)
                                             || Sprite_add(&sprite, 0b10010000)
                                             || Sprite_add(&sprite, 0b10010000)
                                             || Sprite_add(&sprite, 0b11110000)
                                        ) {
                                                fprintf(stderr, "The initialization of the sprite 'O' failed (%s).\n", errorstr());
                                        }
                                        if (Display_DRW(&display, &sprite, 29, 10, &collision)) {
                                                fprintf(stderr, "Display_DRW failed (%s).\n", errorstr());
                                        }
                                        Sprite_clear(&sprite);
                                        // draw !
                                        if (    Sprite_add(&sprite, 0b10000000)
                                             || Sprite_add(&sprite, 0b10000000)
                                             || Sprite_add(&sprite, 0b10000000)
                                             || Sprite_add(&sprite, 0b10000000)
                                             || Sprite_add(&sprite, 0b00000000)
                                             || Sprite_add(&sprite, 0b10000000)
                                        ) {
                                                fprintf(stderr, "The initialization of the sprite '!' failed (%s).\n", errorstr());
                                        }
                                        if (    Display_DRW(&display, &sprite, 34, 10, &collision)
                                             || Display_DRW(&display, &sprite, 36, 10, &collision)
                                             || Display_DRW(&display, &sprite, 38, 10, &collision)
                                        ) {
                                                fprintf(stderr, "Display_DRW failed (%s).\n", errorstr());
                                        }
                                        Sprite_clear(&sprite);
                                        // update the screen
                                        if (Display_update(&display)) {
                                                fprintf(stderr, "Display_update failed (%s).\n", errorstr());
                                        }
                                        // debug info
                                        printf("Iteration: %2d; collision: %d.\n", i, collision);
                                        // blink at about 4 hertz
                                        SDL_Delay(250);
                                }
                                Sprite_destroy(&sprite);
                        }
                }
                Display_destroy(&display);
        }
}
