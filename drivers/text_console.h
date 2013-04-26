//
//  Copyright(C) 2012, Tim Detwiler <timdetwiler@gmail.com>
//
//  This file is part of Timix.
//
//  Timix is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Timix is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Timix.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef TIMIX_TEXT_CONSOLE_H_
#define TIMIX_TEXT_CONSOLE_H_

#include <console.h>
#include <mutex.h>
#include <sem.h>
#include <x86/video_defines.h>


/** @brief The number of characters on the console. */
#define CONSOLE_CHAR_COUNT              (CONSOLE_WIDTH*CONSOLE_HEIGHT)

/** @brief Gets the index of a character. */
#define TERM_CHAR_INDEX(row, col)       ((row)*CONSOLE_WIDTH + (col))

/** @brief Given buffer, row and col, return the address of the character. */
#define TERM_CHAR_ADDRESS(buf,row,col)  \
    ((char*)((buf)->cons_buf + ((row)*CONSOLE_WIDTH + (col))))

/** @brief Where to place to cursor when it's set to be "hidden" */
#define CURSOR_POS_HIDDEN               (CONSOLE_WIDTH * CONSOLE_HEIGHT)

/** @brief The number of bytes used by the console buffer. */
#define CONSOLE_MEM_RANGE               ((2*CONSOLE_WIDTH*CONSOLE_HEIGHT))

/** @brief Flag indicating the cursor is set to be visible. */
#define CURSOR_STATE_VISIBLE            1
/** @brief Flag indicating the cursor is set to be hidden. */
#define CURSOR_STATE_HIDDEN             0
/** @brief The color used to clear the console. */
#define CONSOLE_CLEAR_COLOR             (FGND_BLACK | BGND_BLACK)

/**
 * @brief Structure to represent a single character in memory.
 */
struct console_char {
    /** @brief The ASCII code of the char */
    char cc_code;
    /** @brief The color of the character. */
    char cc_color;
} __attribute__((packed));
typedef struct console_char cons_char_t;

/**
 * @brief Type to hold all information about a console.
 *
 * Tracks state of a console and cursor. Cursor position is measured in x,y
 * values, starting at the top left of the screen and increasing in both
 * directions.
 */
struct console_screen {
    /** @brief Memory buffer where character data is written to. */
    cons_char_t*    cons_buf;
    /** @brief The X coordinate of the cursor. */ 
    int             cons_cur_x;
    /** @brief The Y coordinate of the cursor. */
    int             cons_cur_y;
    /** @brief State, such as cursor hidden/visible. */
    int             cons_cur_state;
    /** @brief Current console color. */
    int             cons_cur_color;
    /** @brief Lock to queue access to the screen. */
    sem_t           cons_lock;
    /** @brief Lock to protect the screen. */
    sem_t           cons_buffer_lock;
};
typedef struct console_screen cons_screen_t;

/**
 * @brief Initializes the console driver.
 *
 * Sets up the screen buffers.
 *
 * @return 0 on success, or a non-zero error code on error.
 */
void console_init(void);

/**
 * @brief Sets the "screen" to be used.
 *
 * The concept of screens is essentially the ability to have multiple parallell
 * console states managed by the console driver. When the application sets the
 * console to a different screen, the console driver will save the console state
 * (text buffer, cursor position/state/color etc) and then load those
 * parameters for the choses screen.
 *
 * @param n The screen ID to be used. Should be in the range of 0 to 
 * CONFIG_SCREEN_COUNT
 * @return 0 on success, or a non-zero error code on error.
 */
int scr_set_screen(int n);

/**
 * @brief Gets the index of the screen that is currently in the forground.
 *
 * @return The screen index, from 0 to CONFIG_SCREEN_COUNT - 1.
 */
int scr_get_screen(void);

#endif // TIMIX_TEXT_CONSOLE_H_
