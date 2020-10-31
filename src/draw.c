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

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

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

CFWAPI void cfw_draw_line(int x1, int y1, int x2, int y2, char c)
{
    CFW_REQUIRE_INIT();

    // Set the initial grid location of the line
    int x = x1;
    int y = y1;

    // Get the distance between the two points
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    // The count of tiles the line intersects
    int tile_count = 1 + dx + dy;

    // The value the x and y should increment with after a tile has
    // been visited
    int x_increment = (x2 > x1) ? 1 : -1;
    int y_increment = (y2 > y1) ? 1 : -1;

    // The difference between the advancement value of the 
    // advancement on the horizontal and vertical axises
    float advance_axis = dy - dx;
    
    // For the math to equal out, the distance must be doubled
    dx *= 2;
    dy *= 2;

    // Traverse line
    for (; tile_count > 0; tile_count--)
    {
        // Draw the line character to the tile position
        _cfw_platform_draw_char(x, y, c);

        // Check what axis should be advanced next
        if (advance_axis == 0)
        {
            // The line passes through both the X and Y axis at the
            // same time
            x += x_increment;
            y += y_increment;
            advance_axis += dy - dx;
            continue;
        }
        else if (advance_axis > 0)
        {
            // Advance on the y axis
            y += y_increment;
            advance_axis -= dx;
        }
        else
        {
            // Advance on the x axis
            x += x_increment;
            advance_axis += dy;
        }
    }
}