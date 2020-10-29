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

void _cfw_platform_draw_str(int x, int y, const char* str)
{
    mvprintw(y, x, str);
}