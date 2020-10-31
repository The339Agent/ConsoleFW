/**
 * @file draw.c
 * @author Nicolai Frigaard
 * @brief Implementation of public draw/render API.
 * 
 * The definition of API calls used for drawing/rendering to the
 * console are found in this file.
 * 
 * @copyright Copyright (c) 2020
 */

#include "internal.h"

// ------------------------------------------------------------------
// |                         CFW PUBLIC API                         |
// ------------------------------------------------------------------

CFWAPI void cfw_clear(void)
{
    CFW_REQUIRE_INIT();
    _cfw_platform_clear();
}

CFWAPI void cfw_draw_char(int x, int y, char c)
{
    CFW_REQUIRE_INIT();
    _cfw_platform_draw_char(x, y, c);
}

CFWAPI void cfw_draw_str(int x, int y, const char* str)
{
    CFW_REQUIRE_INIT();
    _cfw_platform_draw_str(x, y, str);
}