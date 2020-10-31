/**
 * @file ncurses_draw.c
 * @author Nicolai Frigaard
 * @brief The ncurses implementation of platform-specific draw code.
 * 
 * This file contains the ncurses implementation of the
 * platform-specific code.
 * 
 * @copyright Copyright (c) 2020
 */

#include <unistd.h>

#include <ncurses.h>

#include "internal.h"
#include "ncurses_internal.h"

// ------------------------------------------------------------------
// |                        CFW platform API                        |
// ------------------------------------------------------------------

void _cfw_platform_clear(void)
{
    clear();            // Clear the content of the console
}

void _cfw_platform_set_color(int fg_color, int bg_color)
{
    // Set color pair
    attron(COLOR_PAIR(_cfw_platform_ncurses_colornum(fg_color, bg_color)));

    // Check if text is bold
    if (_cfw_platform_ncurses_is_bold(fg_color))
        attron(A_BOLD);
}

void _cfw_platform_unset_color(int fg_color, int bg_color)
{
    // Disable color pair
    attroff(COLOR_PAIR(_cfw_platform_ncurses_colornum(fg_color, bg_color)));
    use_default_colors();

    // Check if text was bold
    if (_cfw_platform_ncurses_is_bold(fg_color))
        attroff(A_BOLD);
}

void _cfw_platform_draw_char(int x, int y, char c)
{
    mvaddch(y, x, c);
}

void _cfw_platform_draw_str(int x, int y, const char* str)
{
    mvprintw(y, x, str);
}