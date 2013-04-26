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

#define VALID_COLOR_MASK    (0xFF)
#define GET_LSB(b) ((b) & 0xFF)
#define GET_MSB(b) (((b) >> 8) & 0xFF)
#define scr_is_console(ptr) ((ptr) == (&console))

//------------------------------------------------------------------------
// Screen-Level Console Driver Interface
//------------------------------------------------------------------------
static void scr_draw_char(cons_screen_t *buf, int row, int col,
                          int ch, int color);
static int scr_putbyte(cons_screen_t *buf, char c);
static void scr_putbytes(cons_screen_t *buf, const char *str, int len);
static void scr_clear_console(cons_screen_t *buf);
static int scr_set_term_color(cons_screen_t *buf,int color);
static void scr_get_term_color(cons_screen_t *buf,int *color);
static int scr_set_cursor(cons_screen_t *buf,int row, int col);
static void scr_get_cursor(cons_screen_t *buf, int *row, int *col);
static void scr_hide_cursor(cons_screen_t *buf);
static void scr_show_cursor(cons_screen_t *buf);
static char scr_get_char(cons_screen_t *buf, int row, int col);


/** @brief The screen buffers structures. */
static cons_screen_t screens[CONFIG_SCREEN_COUNT];

/** @brief The memory areas to be used as console buffers. */
static cons_char_t screen_buffers[CONFIG_SCREEN_COUNT][CONSOLE_CHAR_COUNT];

/** @brief The current screen number that is being displayed. */
static int  screen_idx = 0;

/**
 * @brief Console buffer structure.
 */
static cons_screen_t console = {
    .cons_buf = (cons_char_t*)CONSOLE_MEM_BASE,
    .cons_cur_x = 0,
    .cons_cur_y = 0,
    .cons_cur_state = CURSOR_STATE_VISIBLE,
    .cons_cur_color = (FGND_WHITE | BGND_BLACK)
};

void console_init()
{
    int i;

    sem_init(&(console.cons_lock), 1);
    sem_init(&(console.cons_buffer_lock), 1);
    for(i=0; i<CONFIG_SCREEN_COUNT; ++i) {
        screens[i].cons_buf = screen_buffers[i];
        screens[i].cons_cur_x = 0;
        screens[i].cons_cur_y = 0;
        screens[i].cons_cur_state = CURSOR_STATE_VISIBLE;
        screens[i].cons_cur_color = (FGND_WHITE | BGND_BLACK);
        sem_init(&(screens[i].cons_lock), 1);
        sem_init(&(screens[i].cons_buffer_lock), 1);
    }

    return;
}


/**
 * @brief Sync the hardware cursor with the buffer state.
 *
 * Console manipulation functions only modify the cursor state in the
 * console_screen_t structures. This function commits this state to the CRTC.
 */
static void cursor_sync()
{
    int row = console.cons_cur_y;
    int col = console.cons_cur_x;
    int idx = 0;
    unsigned char c = 0;

    if (console.cons_cur_state == CURSOR_STATE_HIDDEN) {
        row = CONSOLE_HEIGHT;
        col = CONSOLE_WIDTH;
    }

    idx = (row * CONSOLE_WIDTH) + col;

    c = GET_LSB(idx);
    write8(CRTC_IDX_REG, CRTC_CURSOR_LSB_IDX);
    write8(CRTC_DATA_REG, c);

    c = GET_MSB(idx);
    write8(CRTC_IDX_REG, CRTC_CURSOR_MSB_IDX);
    write8(CRTC_DATA_REG, c);

    return;
}

/**
 * @brief Scrools the console back a given number of lines.
 *
 * @param n The number of lines to scrollback.
 */
static void scrollback(cons_screen_t *buf, int n)
{
    int len, row, col;
    int scrollback = n;
    cons_char_t *start;

    len = (CONSOLE_HEIGHT - scrollback) * 2 * CONSOLE_WIDTH;
    scrollback = scrollback * CONSOLE_WIDTH;
    start = &buf->cons_buf[scrollback];

    // Scroll characters down:
    memcpy((void*)buf->cons_buf, start, len);

    // Clear out old line:
    for (row = CONSOLE_HEIGHT - n; row < CONSOLE_HEIGHT; row++) {
        for (col = 0; col<CONSOLE_WIDTH; col++) {
            scr_draw_char(buf, row, col, ' ', CONSOLE_CLEAR_COLOR);
        }
    }

    buf->cons_cur_y -= n;
}

/**
 * @brief Checks the cursor position to make sure it's in a legal position. If
 * not it makes modifications to the cursor state to make it legal.
 *
 * One caveat with scrollback, the driver will scrollback at different times
 * depending on if the cursor is hidden or visible. With a visible cursor
 * scrollback will occur when the last character on the last line is written.
 * When the cursor is not visible, scrollback will not occur until the next
 * character is written. Essentially the driver treats the cursor as an extra
 * character for scrollback purposes.
 */
static void check_cursor(cons_screen_t *buf)
{
    if (buf->cons_cur_x >= CONSOLE_WIDTH) {
        buf->cons_cur_x = 0;
        buf->cons_cur_y += 1;
    }
    if (buf->cons_cur_x < 0) {
        if (buf->cons_cur_y > 0) {
            buf->cons_cur_y -= 1;
            buf->cons_cur_x = CONSOLE_WIDTH-1;
        } else {
            buf->cons_cur_x = 0;
        }
    }

    /* If we have the cursor visible, then that means when the cursor goes off
     * the end of the bottom line, we want the cursor set to col=0 and the term
     * to scrollback. If the cursor is not visible, we don't want to scrollback
     * until we have written a character to the new line.                       */
    if (!(buf->cons_cur_y == CONSOLE_HEIGHT && 
          buf->cons_cur_state == CURSOR_STATE_HIDDEN &&
          buf->cons_cur_x == 0) &&
         (buf->cons_cur_y >= CONSOLE_HEIGHT))
    {
        int n = (buf->cons_cur_y - CONSOLE_HEIGHT) + 1;
        scrollback(buf,n);
    }
}

/**
 * @brief Saves the current state of the console buffer to a background 
 * buffer.
 *
 * @return The index of the screen the buffer was saved to.
 */
static int save_screen()
{
    cons_screen_t *saved = &screens[screen_idx];

    saved->cons_cur_x = console.cons_cur_x;
    saved->cons_cur_y = console.cons_cur_y;
    saved->cons_cur_state = console.cons_cur_state;
    saved->cons_cur_color = console.cons_cur_color;

    memcpy(saved->cons_buf, console.cons_buf, CONSOLE_MEM_RANGE);

    return screen_idx;
}

/**
 * @brief Loads a background buffer into the console buffer.
 *
 * @param id The index of the screen to load.
 * @return 0 on success, or a non-zero error code on error.
 */
static void load_screen(int id)
{
    cons_screen_t *load = &screens[id];

    console.cons_cur_x = load->cons_cur_x;
    console.cons_cur_y = load->cons_cur_y;
    console.cons_cur_state = load->cons_cur_state;
    console.cons_cur_color = load->cons_cur_color;

    memcpy(console.cons_buf, load->cons_buf, CONSOLE_MEM_RANGE);

    cursor_sync();

    return;
}

int scr_set_screen(int n)
{
    sem_t *scr_lock;
    sem_t *old_scr_lock;

    if (n > CONFIG_SCREEN_COUNT)
        return -EINVAL;
    if (n == screen_idx)
        return 0;

    // Using the .cons_buffer_locks allow us to acquire the screen lock without
    // waiting for all the currently queued threads to finish. Worst case we
    // will need to wait for one print operation to finish.
    scr_lock = &(screens[n].cons_buffer_lock);
    old_scr_lock = &(screens[screen_idx].cons_buffer_lock);

    // Currently we only support calling scr_set_screen from a kthread that
    // will never vanish, so we should never get -EINTR from sem_wait.
    assert(sem_wait(scr_lock) == 0);
    assert(sem_wait(old_scr_lock) == 0);

    save_screen();
    load_screen(n);
    screen_idx = n;

    sem_signal(old_scr_lock);
    sem_signal(scr_lock);

    return 0;
}

int scr_get_screen()
{
    return screen_idx;
}

void scr_draw_char(cons_screen_t *buf, int row, int col, int ch, int color)
{
    int index;

    if (row >= CONSOLE_HEIGHT || col >= CONSOLE_WIDTH)
        return;

    index = TERM_CHAR_INDEX(row,col);
    buf->cons_buf[index].cc_code = ch;
    buf->cons_buf[index].cc_color = color;

    return;
}

int scr_putbyte(cons_screen_t *buf, char c)
{
    switch (c) {
    case '\n':
        buf->cons_cur_x = 0;
        buf->cons_cur_y += 1;
        break;
    case '\r':
        buf->cons_cur_x = 0;
        break;
    case '\b':
        buf->cons_cur_x -= 1;
        break;
    default: 
        scr_draw_char(buf,buf->cons_cur_y, buf->cons_cur_x, c, buf->cons_cur_color);
        buf->cons_cur_x += 1;
        if (buf->cons_cur_x >= CONSOLE_WIDTH) {
            buf->cons_cur_x = 0;
            buf->cons_cur_y += 1;
        }
    }

    check_cursor(buf);
    scr_draw_char(buf,buf->cons_cur_y, buf->cons_cur_x, ' ',
                  buf->cons_cur_color);

    return c;
}

void scr_putbytes(cons_screen_t *buf, const char *str, int len)
{
    int i;

    for (i=0; i<len; ++i) {
        scr_putbyte(buf,str[i]);
    }

    return;
}


void scr_clear_console(cons_screen_t *buf)
{
    int row, col;

    for (row=0; row<CONSOLE_HEIGHT; ++row) {
        for (col=0; col<CONSOLE_WIDTH; ++col) {
            scr_draw_char(buf, row, col, ' ', CONSOLE_CLEAR_COLOR);
        }
    }

    return;
}

int scr_set_term_color(cons_screen_t *buf,int color)
{
    buf->cons_cur_color = color;

    return 0;
}

void scr_get_term_color(cons_screen_t *buf,int *color)
{
    *color = buf->cons_cur_color;

    return;
}

int scr_set_cursor(cons_screen_t *buf,int row, int col)
{
    if (row < 0 || col < 0)
        return -1;

    if (row >= CONSOLE_HEIGHT || col >= CONSOLE_WIDTH)
        return -1;

    buf->cons_cur_x = col;
    buf->cons_cur_y = row;

    return 0;
}

void scr_get_cursor(cons_screen_t *buf, int *row, int *col)
{
    if (buf->cons_cur_state != CURSOR_STATE_VISIBLE && 
        buf->cons_cur_y >= CONSOLE_WIDTH)
    {
        // The way scrollback is implemented means we have a special case here
        *row = buf->cons_cur_y+1;
        *col = 0;   
    } else {
        *row = buf->cons_cur_y;
        *col = buf->cons_cur_x;
    }

    return;
}

void scr_hide_cursor(cons_screen_t *buf)
{
    buf->cons_cur_state = CURSOR_STATE_HIDDEN;

    return;
}

void scr_show_cursor(cons_screen_t *buf)
{
    buf->cons_cur_state = CURSOR_STATE_VISIBLE;

    return;
}

char scr_get_char(cons_screen_t *buf, int row, int col)
{
    int index;
    char c;

    if (row < 0 || col < 0 || row >= CONSOLE_HEIGHT || col >= CONSOLE_HEIGHT)
        return '\0';

    index = TERM_CHAR_INDEX(row,col);
    c = buf->cons_buf[index].cc_code;

    return c;
}

cons_screen_t *scr_lock_thread_screen()
{
    int n;
    cons_screen_t *cons = &console;
    cons_screen_t *screen;
    sem_t *cons_lock;
    sem_t *cons_buf_lock;

    n = current_thread->screen;
    screen = &(screens[n]);
    cons_lock = &(screen->cons_lock);
    cons_buf_lock = &(screen->cons_buffer_lock);
    if (sem_wait(cons_lock) == -EINTR)
        return NULL;
    if (sem_wait(cons_buf_lock) == -EINTR) {
        sem_signal(cons_lock);
        return NULL;
    }
    if (current_thread->screen == screen_idx)
        return cons;
    else
        return &(screens[current_thread->screen]);
}

void scr_unlock_screen() {
    int n = current_thread->screen;

    sem_signal(&(screens[n].cons_buffer_lock));
    sem_signal(&(screens[n].cons_lock));
}

int putbyte(char c)
{
    cons_screen_t *buf;

    buf = scr_lock_thread_screen();
    if (buf == NULL)
        return -EINTR;
    scr_putbyte(buf, c);

    if (scr_is_console(buf))
        cursor_sync();
    scr_unlock_screen();

    return c;
}

int putbytes(const char *str, int len)
{
    cons_screen_t *buf;

    buf = scr_lock_thread_screen();
    if (buf == NULL)
        return -EINTR;
    scr_putbytes(buf, str, len);
    if (scr_is_console(buf))
        cursor_sync();
    scr_unlock_screen();

    return 0;
}

void draw_char(int row, int col, int ch, int color)
{
    cons_screen_t *buf;

    buf = scr_lock_thread_screen();
    if (buf == NULL)
        return;
    scr_draw_char(buf, row, col, ch, color);

    if (scr_is_console(buf))
        cursor_sync();
    scr_unlock_screen();

    return;
}

void clear_console()
{
    cons_screen_t *buf;

    buf = scr_lock_thread_screen();
    if (buf == NULL)
        return;
    scr_clear_console(buf);
    scr_unlock_screen();


    return;
}

int cons_set_term_color(int color)
{
    cons_screen_t *buf;
    
    if(color < 0 || (color & ~VALID_COLOR_MASK)) {
        return -EINVAL;
    }

    buf = scr_lock_thread_screen();
    if (buf == NULL)
        return -EINTR;
    scr_set_term_color(buf, color);
    scr_unlock_screen();


    return 0;
}

int get_term_color(int *color)
{
    cons_screen_t *buf;

    buf = scr_lock_thread_screen();
    if (buf == NULL)
        return -EINTR;

    scr_get_term_color(buf, color);
    scr_unlock_screen();

    return 0;
}

int set_cursor(int row, int col)
{
    int ret;
    cons_screen_t *buf;

    buf = scr_lock_thread_screen();
    if (buf == NULL)
        return -EINTR;

    ret = scr_set_cursor(buf, row, col);

    if (scr_is_console(buf))
        cursor_sync();
    scr_unlock_screen();

    return ret;
}

int get_cursor(int *row, int *col)
{
    cons_screen_t *buf;

    buf = scr_lock_thread_screen();
    if (buf == NULL)
        return -EINTR;

    scr_get_cursor(buf, row, col);
    scr_unlock_screen();


    return 0;
}

int hide_cursor()
{
    cons_screen_t *buf;

    buf = scr_lock_thread_screen();
    if (buf == NULL)
        return -EINTR;

    scr_hide_cursor(buf);

    if (scr_is_console(buf))
        cursor_sync();
    scr_unlock_screen();


    return 0;
}

int show_cursor()
{
    cons_screen_t *buf;

    buf = scr_lock_thread_screen();
    if (buf == NULL)
        return -EINTR;

    scr_show_cursor(buf);

    if (scr_is_console(buf))
        cursor_sync();
    scr_unlock_screen();


    return 0;
}

int get_char(int row, int col)
{
    char c;
    cons_screen_t *buf;

    buf = scr_lock_thread_screen();
    if (buf == NULL)
        return -EINTR;

    c = scr_get_char(buf, row, col);
    scr_unlock_screen();


    return c;
}
