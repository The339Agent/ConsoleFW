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
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "internal.h"

int translate_xy_to_bounds(int *x, int *y, int length)
{
    int max_x, max_y;
    int offset_x = 0, offset_y = 0;
    max_x = __cfw.width;
    max_y = __cfw.height;

    // If a region is currently set...
    if (__cfw.region_head != NULL)
    {
        // ...translate the position in the bounds

        // Get the bound data of the currently active bounds
        {
            __cfw_region **current = &__cfw.region_head;

            while (*current != NULL)
            {
                offset_x += (*current)->x;
                offset_y += (*current)->y;
                max_x = min(max_x, (*current)->width);
                max_y = min(max_y, (*current)->height);

                current = &(*current)->next;
            }
        }

        // Shift XY
        *x += offset_x;
        *y += offset_y;

        // Check if the Y overflows the bound
        if (*y > max_y + offset_y)
            // Return a ridiculous overlow value
            return 0xFFFFFF;
    }

    // Return the count of characters overflow the bound
    return max(0, *x + length - (max_x + offset_x));
}

// Polygon draw calls

void draw_line(int x1, int y1, int x2, int y2, char c)
{
    CFW_REQUIRE_INIT();

    // Get the distance between the two points
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    // Set the initial grid location of the line
    int x = x1;
    int y = y1;

    // The count of tiles the line intersects
    int tile_count = 1 + dx + dy;

    // The value the x and y should increment with after a tile has
    // been visited
    int x_increment = (x2 > x1) ? 1 : -1;
    int y_increment = (y2 > y1) ? 1 : -1;

    // The difference between the advancement value of the 
    // advancement on the horizontal and vertical axises
    int advance_axis = dx - dy;
    
    // For the math to equal out, the distance must be doubled
    dx *= 2;
    dy *= 2;

    // Traverse line
    for (; tile_count > 0; --tile_count)
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
            advance_axis += dx - dy;
            --tile_count;
        }
        else if (advance_axis > 0)
        {
            // Advance on the x axis
            x += x_increment;
            advance_axis -= dy;
        }
        else
        {
            // Advance on the y axis
            y += y_increment;
            advance_axis += dx;
        }
    }
}

void draw_triangle_lines(int x1, int y1,
                         int x2, int y2,
                         int x3, int y3, char c)
{
    cfw_draw_line(x1, y1, x2, y2, c);
    cfw_draw_line(x2, y2, x3, y3, c);
    cfw_draw_line(x3, y3, x1, y1, c);
}

void draw_bottom_triangle_fill(int x1, int y1,
                               int x2, int y2,
                               int x3, int y3, char c)
{
    float invslope1 = (x2 - x1) / (y2 - y1);
    float invslope2 = (x3 - x1) / (y3 - y1);

    float curx1 = x1;
    float curx2 = x1;

    for (int scanline_y = y1; scanline_y <= y2; scanline_y++)
    {
        cfw_draw_line((int)curx1, scanline_y, (int)curx2, scanline_y, c);
        curx1 += invslope1;
        curx2 += invslope2;
    }
}

void draw_top_triangle_fill(int x1, int y1,
                            int x2, int y2,
                            int x3, int y3, char c)
{
    float invslope1 = (x3 - x1) / (y3 - y1);
    float invslope2 = (x3 - x2) / (y3 - y2);

    float curx1 = x3;
    float curx2 = x3;

    for (int scanline_y = y3; scanline_y > y1; scanline_y--)
    {
        cfw_draw_line((int)curx1, scanline_y, (int)curx2, scanline_y, c);
        curx1 -= invslope1;
        curx2 -= invslope2;
    }
}

void draw_triangle_fill(int x1, int y1,
                        int x2, int y2,
                        int x3, int y3, char c)
{
    // Sort the points by accending y coordinate
    if (y1 > y2) { CFW_SWAP_VALUES(x1, x2); CFW_SWAP_VALUES(y1, y2); }
    if (y1 > y3) { CFW_SWAP_VALUES(x1, x3); CFW_SWAP_VALUES(y1, y3); }
    if (y2 > y3) { CFW_SWAP_VALUES(x2, x3); CFW_SWAP_VALUES(y2, y3); }

    // Check if the triangle is either a top triangle or a bottom
    // triangle
    if (y2 == y3)
    {
        // The triangle is a bottom triangle
        draw_bottom_triangle_fill(x1, y1, x2, y2, x3, y3, c);
    }
    else if (y1 == y2)
    {
        // The triangle is a top triangle
        draw_top_triangle_fill(x1, y1, x2, y2, x3, y3, c);
    }
    else
    {
        // The triangle is neither, so split it into both types
        int x4 = (int)(x1 + ((float)(y2 - y1) / (float)(y3 - y1)) * (x3 - x1));
        int y4 = y2;
        draw_bottom_triangle_fill(x1, y1, x2, y2, x4, y4, c);
        draw_top_triangle_fill(x2, y2, x4, y4, x3, y3, c);
    }
    
}

void draw_quad_lines(int x1, int y1,
                     int x2, int y2,
                     int x3, int y3,
                     int x4, int y4, char c)
{
    cfw_draw_line(x1, y1, x2, y2, c);
    cfw_draw_line(x2, y2, x3, y3, c);
    cfw_draw_line(x3, y3, x4, y4, c);
    cfw_draw_line(x4, y4, x1, y1, c);
}

void draw_quad_fill(int x1, int y1,
                    int x2, int y2,
                    int x3, int y3,
                    int x4, int y4, char c)
{
    draw_triangle_fill(x1, y1, x2, y2, x3, y3, c);
    draw_triangle_fill(x1, y1, x3, y3, x4, y4, c);
}

// DISCLAIMER: The circle rasterization code is from the
// OneLoneCoders olcConsoleGameEngine project.
//
// Repository:
// https://github.com/OneLoneCoder/videos/blob/master/olcConsoleGameEngine.h

void draw_circle_lines(int x, int y, int radius, char c)
{
    int tile_x = 0;
    int tile_y = radius;
    int f = 3 - 2 * radius;

    if (!radius) return;
    
    while (tile_y >= tile_x)
    {
        _cfw_platform_draw_char(x - tile_x, y - tile_y, c);
        _cfw_platform_draw_char(x - tile_y, y - tile_x, c);
        _cfw_platform_draw_char(x + tile_y, y - tile_x, c);
        _cfw_platform_draw_char(x + tile_x, y - tile_y, c);
        _cfw_platform_draw_char(x - tile_x, y + tile_y, c);
        _cfw_platform_draw_char(x - tile_y, y + tile_x, c);
        _cfw_platform_draw_char(x + tile_y, y + tile_x, c);
        _cfw_platform_draw_char(x + tile_x, y + tile_y, c);
        if (f < 0) f += 4 * tile_x++ + 6;
        else f += 4 * (tile_x++ - tile_y--) + 10;
    }
}

void draw_strait(int sx, int ex, int ny, char c)
{
    for (int i = sx; i <= ex; i++)
        _cfw_platform_draw_char(i, ny, c);
}

void draw_circle_fill(int x, int y, int radius, char c)
{
    int tile_x = 0;
    int tile_y = radius;
    int f = 3 - 2 * radius;

    if (!radius) return;
    
    while (tile_y >= tile_x)
    {
        draw_strait(x - tile_x, x + tile_x, y - tile_y, c);
        draw_strait(x - tile_y, x + tile_y, y - tile_x, c);
        draw_strait(x - tile_x, x + tile_x, y + tile_y, c);
        draw_strait(x - tile_y, x + tile_y, y + tile_x, c);
        if (f < 0) f += 4 * tile_x++ + 6;
        else f += 4 * (tile_x++ - tile_y--) + 10;
    }
}

// End of OneLoneCoder code

// ------------------------------------------------------------------
// |                         CFW PUBLIC API                         |
// ------------------------------------------------------------------

CFWAPI void cfw_clear(void)
{
    CFW_REQUIRE_INIT();
    _cfw_platform_clear();
}

CFWAPI void cfw_polygon_mode(int mode)
{
    CFW_REQUIRE_INIT();

    // Check if the mode is not one of the permitted modes
    if (mode < CFW_POINTS || mode > CFW_FILL)
    {
        _cfw_input_error(CFW_INVALID_VALUE, "Mode 0x%x is not a valid polygon mode.", mode);
        return;
    }

    // Set the polygon mode
    __cfw.polygon_mode = mode;
}

CFWAPI void cfw_set_default_color(void)
{
    CFW_REQUIRE_INIT();
    CFW_REQUIRE_FEATURE_ENABLED(CFW_COLORS);

    // Check if the color is valid
    if (__cfw.foreground_color != -1 && __cfw.background_color != -1)
        _cfw_platform_unset_color(__cfw.foreground_color, __cfw.background_color);

    // Set default colors
    __cfw.foreground_color = CFW_WHITE;
    __cfw.background_color = CFW_BLACK;

    // There is no need to set the new colors, as they are already
    // enabled when the previously bound color is unbound.
}

CFWAPI void cfw_set_color(int foreground_color, int background_color)
{
    CFW_REQUIRE_INIT();
    CFW_REQUIRE_FEATURE_ENABLED(CFW_COLORS);

    // Check if the color is valid
    if (foreground_color < CFW_BLACK || foreground_color > CFW_BOLD_WHITE)
    {
        _cfw_input_error(CFW_INVALID_VALUE, "%d is not a valid foreground color.",
                         foreground_color);
        return;
    }
    else if (background_color < CFW_BLACK || background_color > CFW_WHITE)
    {
        _cfw_input_error(CFW_INVALID_VALUE, "%d is not a valid foreground color.",
                         background_color);
        return;
    }

    _cfw_platform_unset_color(__cfw.foreground_color, __cfw.background_color);

    __cfw.foreground_color = foreground_color;
    __cfw.background_color = background_color;

    _cfw_platform_set_color(__cfw.foreground_color, __cfw.background_color);
}

CFWAPI void cfw_set_foreground_color(int color)
{
    CFW_REQUIRE_INIT();
    CFW_REQUIRE_FEATURE_ENABLED(CFW_COLORS);

    // Check if the color is valid
    if (color < CFW_BLACK || color > CFW_BOLD_WHITE)
    {
        _cfw_input_error(CFW_INVALID_VALUE, "%d is not a valid color.", color);
        return;
    }

    _cfw_platform_unset_color(__cfw.foreground_color, __cfw.background_color);
    
    __cfw.foreground_color = color;

    _cfw_platform_set_color(__cfw.foreground_color, __cfw.background_color);
}

CFWAPI void cfw_set_background_color(int color)
{
    CFW_REQUIRE_INIT();
    CFW_REQUIRE_FEATURE_ENABLED(CFW_COLORS);

    // Check if color is valid
    if (color < CFW_BLACK || color > CFW_WHITE)
    {
        _cfw_input_error(CFW_INVALID_VALUE, "%d is not a valid color.", color);
        return;
    }

    _cfw_platform_unset_color(__cfw.foreground_color, __cfw.background_color);

    __cfw.background_color = color;

    _cfw_platform_set_color(__cfw.foreground_color, __cfw.background_color);
}

CFWAPI void cfw_begin_region(int x, int y, int width, int height)
{
    CFW_REQUIRE_INIT();

    // Allocate memory for region
    __cfw_region *region = malloc(sizeof(__cfw_region));

    // Set region bounds
    region->x = x;
    region->y = y;
    region->width = width;
    region->height = height;

    // Save region
    region->next = __cfw.region_head;
    __cfw.region_head = region;
}

CFWAPI void cfw_end_region(void)
{
    CFW_REQUIRE_INIT();

    // If no region is currently set...
    if (__cfw.region_head == NULL) return; // ... do nothing

    // Pop region list
    __cfw_region *region = __cfw.region_head;
    __cfw.region_head = region->next;

    // Delete region
    free(region);
}

CFWAPI void cfw_get_region_bounds(int *width, int *height)
{
    CFW_REQUIRE_INIT();

    // If no region is currently set...
    if (__cfw.region_head == NULL)
    {
        // ...return the size of the console
        cfw_get_console_size(width, height);
    }
    else
    {
        // Get the size of the topmost region
        __cfw_region *region = __cfw.region_head;
        if (width != NULL)  *width  = region->width;
        if (height != NULL) *height = region->height;
    }
}

CFWAPI void cfw_draw_char(int x, int y, char c)
{
    CFW_REQUIRE_INIT();

    // Translate the XY to the current bounds
    int overflow = translate_xy_to_bounds(&x, &y, 1);
    if (overflow > 0) return;

    // Draw the character
    _cfw_platform_draw_char(x, y, c);
}

CFWAPI void cfw_draw_str(int x, int y, const char *str)
{
    CFW_REQUIRE_INIT();

    // Translate the XY to the current bounds
    int _length = strlen(str);
    int overflow = translate_xy_to_bounds(&x, &y, _length);
    if (overflow >= _length)
        return; // The entire string is out of bounds, discard it.
    else if (overflow > 0)
    {
        // Some, but not all of the characters fit. Only draw the
        // characters that fit.

        // A maximum of 1024 chars are allowed to be included after
        // an overflow. If more than 1024 chars are overflowing the
        // current region, it will be discarded. However, if you have
        // more than 1024 chars overflowing, you have bigger
        // problems.
        char substr[1024];
        substr(substr, str, 0, _length - overflow);
        _cfw_platform_draw_str(x, y, substr);
    }
    else
        // Draw the string in its entirety
        _cfw_platform_draw_str(x, y, str);
}

CFWAPI void cfw_draw_fmt_str(int x, int y, const char *fmt, ...)
{
    // The longest possible formatted string is 1024 chars long.
    char message[1024];

    // Format the string
    va_list va;
    va_start(va, fmt);
    vsnprintf(message, sizeof(message), fmt, va);
    va_end(va);

    // Draw the formatted string
    cfw_draw_str(x, y, message);
}

CFWAPI void cfw_draw_line(int x1, int y1, int x2, int y2, char c)
{
    switch (__cfw.polygon_mode)
    {
    case CFW_POINTS:
        _cfw_platform_draw_char(x1, y1, c);
        _cfw_platform_draw_char(x2, y2, c);
        break;
    case CFW_LINES:
    case CFW_FILL:
        // For a line, FILL and LINES give the same result.
        draw_line(x1, y1, x2, y2, c);
        break;

    default:
        break;
    }
}

CFWAPI void cfw_draw_triangle(int x1, int y1, int x2, int y2,
                              int x3, int y3, char c)
{
    switch (__cfw.polygon_mode)
    {
    case CFW_POINTS:
        _cfw_platform_draw_char(x1, y1, c);
        _cfw_platform_draw_char(x2, y2, c);
        _cfw_platform_draw_char(x3, y3, c);
        break;
    case CFW_LINES:
        draw_triangle_lines(x1, y1, x2, y2, x3, y3, c);
        break;
    case CFW_FILL:
        draw_triangle_fill(x1, y1, x2, y2, x3, y3, c);
        break;

    default:
        break;
    }
}

CFWAPI void cfw_draw_quad(int x1, int y1, int x2, int y2,
                          int x3, int y3, int x4, int y4, char c)
{
    switch (__cfw.polygon_mode)
    {
    case CFW_POINTS:
        _cfw_platform_draw_char(x1, y1, c);
        _cfw_platform_draw_char(x2, y2, c);
        _cfw_platform_draw_char(x3, y3, c);
        _cfw_platform_draw_char(x4, y4, c);
        break;
    case CFW_LINES:
        draw_quad_lines(x1, y1, x2, y2, x3, y3, x4, y4, c);
        break;
    case CFW_FILL:
        draw_quad_fill(x1, y1, x2, y2, x3, y3, x4, y4, c);
        break;

    default:
        break;
    }
}

CFWAPI void cfw_draw_circle(int x, int y, int radius, char c)
{
    switch (__cfw.polygon_mode)
    {
    case CFW_POINTS:
        _cfw_platform_draw_char(x, y, c);
        break;
    case CFW_LINES:
        draw_circle_lines(x, y, radius, c);
        break;
    case CFW_FILL:
        draw_circle_fill(x, y, radius, c);
        break;

    default:
        break;
    }
}
