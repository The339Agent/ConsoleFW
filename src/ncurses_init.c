#include <stdlib.h>

#include <ncurses.h>

#include "internal.h"

// ------------------------------------------------------------------
// |                        CFW platform API                        |
// ------------------------------------------------------------------

CFWAPI cfw__bool _cfw_platform_init(void)
{
    initscr();          // Initialize the window
    noecho();           // Don't echo any keypress
    curs_set(FALSE);    // Don't display a cursor

    // Add the terminate call to an atexit to make sure that ncurses
    // is terminated at the absolute end of the application. If
    // ncurses isn't terminated at the end of execution, it may
    // retake control of the console.
    atexit(_cfw_platform_terminate);

    return CFW_TRUE;
}

CFWAPI void _cfw_platform_terminate(void)
{
    endwin();           // Restore window to normal behavior
}

CFWAPI void _cfw_platform_refresh(void)
{
    refresh();          // Refresh the window and update content
}