/**
 * @file ncurses_init.c
 * @author Nicolai Frigaard
 * @brief The ncurses implementation of platform-specific init code.
 * 
 * This file contains the ncurses implementation of the
 * platform-specific init code.
 * 
 * @copyright Copyright (c) 2020
 */

#include <stdlib.h>

#include <ncurses.h>

#include "internal.h"
#include "ncurses_internal.h"

short get_ncurses_color_id(int color)
{
    switch (color)
    {
    case 0:
        return COLOR_BLACK;
    case 1:
        return COLOR_BLUE;
    case 2:
        return COLOR_GREEN;
    case 3:
        return COLOR_CYAN;
    case 4:
        return COLOR_RED;
    case 5:
        return COLOR_MAGENTA;
    case 6:
        return COLOR_YELLOW;
    case 7:
        return COLOR_WHITE;

    default:
        return COLOR_WHITE;
    }
}

void create_color_pairs()
{
    // Generate all possible color configurations
    for (int bg_color = CFW_BLACK; bg_color <= CFW_WHITE; bg_color++)
    {
        for (int fg_color = CFW_BLACK; fg_color <= CFW_WHITE; fg_color++)
        {
            int id = _cfw_platform_ncurses_colornum(fg_color, bg_color);
            init_pair(id, get_ncurses_color_id(fg_color),
                          get_ncurses_color_id(bg_color));
        }
    }
}

// ------------------------------------------------------------------
// |                   CFW internal platform API                    |
// ------------------------------------------------------------------

int _cfw_platform_ncurses_colornum(int foreground, int background)
{
    // 10001111 = Bold white, black background = 143
    // ||||||||
    // |||||--- Foreground color
    // ||||---- Bold (not set here)
    // |------- Background color
    // -------- Bonus bit to assure that 0 is never returned

    int B, bbb, ffff;

    B = 1 << 7;
    bbb = (7 & background) << 4;
    ffff = 7 & foreground;

    return (B | bbb | ffff);
}

cfw__bool _cfw_platform_ncurses_is_bold(int color)
{
    int i = 1 << 3;
    return (i & color);
}

// ------------------------------------------------------------------
// |                        CFW platform API                        |
// ------------------------------------------------------------------

cfw__bool _cfw_platform_init(void)
{
    initscr();              // Initialize the window
    noecho();               // Don't echo any keypress
    curs_set(FALSE);        // Don't display a cursor
    keypad(stdscr, TRUE);   // Get a single value for special keys

    // Add the terminate call to an atexit to make sure that ncurses
    // is terminated at the absolute end of the application. If
    // ncurses isn't terminated at the end of execution, it may
    // retake control of the console.
    atexit(_cfw_platform_terminate);

    return CFW_TRUE;
}

void _cfw_platform_terminate(void)
{
    endwin();           // Restore window to normal behavior
}

void _cfw_platform_refresh(void)
{
    refresh();          // Refresh the window and update content
}

cfw__bool _cfw_platform_is_feature_supported(int feature)
{
    switch (feature)
    {
    case CFW_COLORS:
        return has_colors();
        break;
    
    default:
        _cfw_input_error(CFW_INVALID_VALUE, "0x%x is not a valid feature.", feature);
        return CFW_FALSE;
    }
}

void _cfw_platform_enable(int feature)
{
    switch (feature)
    {
    case CFW_COLORS:
        start_color();
        create_color_pairs();
        break;

    default:
        _cfw_input_error(CFW_INVALID_VALUE, "0x%x is not a valid feature.", feature);
    }
}

void _cfw_platform_get_console_size(int *width, int *height)
{
    getmaxyx(stdscr, *height, *width);
}