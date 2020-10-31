/**
 * @file cfw.h
 * @author Nicolai Frigaard
 * @brief ConsoleFW header file.
 * 
 * This file is the header file of ConsoleFW (CFW). All public API in
 * CFW is available through including this header.
 * 
 * @copyright Copyright (c) 2020
 */

#ifndef __cfw_include_h__
#define __cfw_include_h__

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

// ------------------------------------------------------------------
// |                       Preprocessor work                        |
// ------------------------------------------------------------------

// Set a single define for Windows
#if !defined(_WIN32) && (defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__))
    #define _WIN32
#endif

#if defined(CFW_DLL) && defined(_CFW_BUILD_DLL)
    // CFW_DLL is for linking against CFW as a shared libary, and
    // _CFW_BUILD_DLL is for building the CFW sources as a shared
    // library. You cannot both use CFW as a shared library and
    // compile it as one as well. Therefore, if both are defined,
    // something is wrong.
    #error "You cannot have both CFW_DLL and _CFW_BUILD_DLL defined"
#endif

// CFWAPI is used to declare public API functionality for CFW as a
// shared library / dynamic library.
#if defined(_WIN32) && defined(_CFW_BUILD_DLL)
    // Building CFW as a Windows DLL
    #define CFWAPI __declspec(dllexport)
#elif defined(_WIN32) && defined(CFW_DLL)
    // Using CFW as a Windows DLL
    #define CFWAPI __declspec(dllimport)
#elif defined(__GNUC__) && defined(_CFW_BUILD_DLL)
    // Building CFW as a shared / dynamic library
    #define CFWAPI __attribute__((visibility("default")))
#else
    // Building or using CFW as a static library
    #define CFWAPI
#endif

// ------------------------------------------------------------------
// |                            CFW API                             |
// ------------------------------------------------------------------

/**
 * @brief The value 0, or false.
 * 
 * Pretty way of writing false in CFW.
 */
#define CFW_FALSE 0

/**
 * @brief The value 1, or false.
 * 
 * Pretty way of writing false in true.
 */
#define CFW_TRUE 1

/**
 * @brief Color support.
 * 
 * The value to pass to `cfw_is_feature_supported()` to check the
 * console for color support. If the console has color support, you
 * can draw to it with colors. If not, the console only supports it's
 * native colors.
 * 
 * Most modern terminals support color. Only outdated and/or very old
 * terminals do not have colors supported.
 * 
 * @code
 * cfw__bool color_supported = cfw_is_feature_supported(CFW_COLORS);
 * @endcode
 */
#define CFW_COLORS  0x00010001

/**
 * @brief Points polygon mode.
 * 
 * Polygons have their points drawn.
 */
#define CFW_POINTS  0x00020001

/**
 * @brief Lines polygon mode.
 * 
 * Boundaries of polygons are drawn as lines.
 */
#define CFW_LINES   0x00020002

/**
 * @brief Fill polygon mode.
 * 
 * The entirety of the polygon is drawn, including the interior.
 */
#define CFW_FILL    0x00020003

/**
 * @brief A boolean value.
 * 
 * This is just an int masked as a boolean used where CFW expects the
 * int to represent a boolean value.
 */
typedef int cfw__bool;

/**
 * @brief Initialize CFW.
 * 
 * This function initializes CFW. Before CFW can be used it must be
 * initialized. Before you terminate your application CFW should be
 * terminated as well to free any allocated resources and free the
 * console.
 * 
 * If initialization fails, CFW will be terminated before returning.
 * 
 * If CFW is already initialized when this function is called, it
 * will just return `CFW_TRUE` immediately.
 * 
 * @return `CFW_TRUE` if initialization was successfull, `CFW_FALSE`
 * if an error occurred. 
 */
CFWAPI cfw__bool cfw_init(void);

/**
 * @brief Terminate CFW.
 * 
 * This function terminates CFW adn frees any resources used by CFW.
 * 
 * If CFW has been initialized, this function should be called before
 * terminating your application. If initialization has failed, this
 * function will have already been called.
 * 
 * If the application isn't initialized, this function will return
 * immediately without doing anything.
 */
CFWAPI void cfw_terminate(void);

/**
 * @brief Refresh the console.
 * 
 * This function refreshes the content of the console.
 */
CFWAPI void cfw_refresh(void);

/**
 * @brief Check if ConsoleFW supports the queried feature.
 * 
 * This function checks if CFW supports the feature provided in the
 * feature parameter. If the value is invalid, the function returns
 * `CFW_FALSE`. If CFW isn't initialized, the function returns
 * `CFW_FALSE`.
 * 
 * Flags can be:
 * - CFW_COLORS
 * 
 * @param feature The feature to check if is supported by the
 * console.
 * @return `CFW_TRUE` if CFW supports the feature, `CFW_FALSE` if it
 * isn't supported. 
 */
CFWAPI cfw__bool cfw_is_feature_supported(int feature);

/**
 * @brief Get the current size of the console
 * 
 * This function gets the current max size of the console and stores
 * it in the provided int pointers.
 * 
 * @param width Pointer to the int to store the console width in.
 * @param height Pointer to the int to store the console height in.
 */
CFWAPI void cfw_get_console_size(int *width, int *height);

/**
 * @brief Clear the console content.
 * 
 * This function clears all content in the console.
 */
CFWAPI void cfw_clear(void);

/**
 * @brief Set the way polygons are rasterized.
 * 
 * This function specifies the way polygons draw in draw calls are
 * rasterized.
 * 
 * Can be `CFW_POINTS`, `CFW_LINES` or `CFW_FILL`.
 * 
 * @param mode Specifies how polygons are rasterized.
 */
CFWAPI void cfw_polygon_mode(int mode);

/**
 * @brief Draw a character to the console.
 * 
 * This function draws the provided character to the console at the
 * given position.
 * 
 * @param x The X position to draw the char at.
 * @param y The Y position to draw the char at.
 * @param c The character to draw at the given position.
 */
CFWAPI void cfw_draw_char(int x, int y, char c);

/**
 * @brief Draw a string to the console.
 * 
 * This function draws the provided string to the console at the
 * given position.
 * 
 * @param x The X position to start drawing the string to.
 * @param y The Y position to start drawing the string to.
 * @param str The string to draw at the given position.
 */
CFWAPI void cfw_draw_str(int x, int y, const char* str);

/**
 * @brief Draw a line polygon to the console.
 * 
 * This function draws a line of the set character to the console.
 * 
 * The line is drawn from the given start position to the given end
 * position.
 * 
 * @param x1 The X position of the start of the line.
 * @param y1 The Y position of the start of the line.
 * @param x2 The X position of the end of the line.
 * @param y2 The Y position of the end of the line.
 * @param c The character to draw the line with.
 */
CFWAPI void cfw_draw_line(int x1, int y1, int x2, int y2, char c);

/**
 * @brief Draw a triangle polygon to the console.
 * 
 * This function draws a triange of the set character to the console.
 * 
 * The triangle is drawn between the three points given.
 * 
 * @param x1 The X position of the first point.
 * @param y1 The Y position of the first point.
 * @param x2 The X position of the second point.
 * @param y2 The Y position of the second point.
 * @param x3 The X position of the third point.
 * @param y3 The Y position of the third point.
 * @param c The character to draw the triangle with.
 */
CFWAPI void cfw_draw_triangle(int x1, int y1, int x2,
                              int y2, int x3, int y3, char c);

/**
 * @brief Draw a quad polygon to the console.
 * 
 * This function draws a quad of the set character to the console.
 * 
 * The quad is drawn within the area defined by the four given
 * points.
 * 
 * @param x1 The X position of the first point.
 * @param y1 The Y position of the first point.
 * @param x2 The X position of the second point.
 * @param y2 The Y position of the second point.
 * @param x3 The X position of the third point.
 * @param y3 The Y position of the third point.
 * @param x4 The X position of the fourth point.
 * @param y4 The Y position of the fourth point.
 * @param c The character to draw the quad with.
 */
CFWAPI void cfw_draw_quad(int x1, int y1,
                          int x2, int y2,
                          int x3, int y3,
                          int x4, int y4, char c);

/**
 * @brief Draw a circle polygon to the console.
 * 
 * This function draws a circle of the set character to the console.
 * 
 * @param x The X position of the center of the circle.
 * @param y The Y position of the center of the circle.
 * @param radius The radius of the circle.
 * @param c The character to draw the circle with.
 */
CFWAPI void cfw_draw_circle(int x, int y, int radius, char c);

#ifdef __cplusplus
}
#endif

#endif /* __cfw_include_h__ */