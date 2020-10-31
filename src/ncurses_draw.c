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

// ------------------------------------------------------------------
// |                        CFW platform API                        |
// ------------------------------------------------------------------

void _cfw_platform_clear(void)
{
    clear();            // Clear the content of the console
}

void _cfw_platform_draw_char(int x, int y, char c)
{
    mvprintw(y, x, &c);
}

void _cfw_platform_draw_str(int x, int y, const char* str)
{
    mvprintw(y, x, str);
}