/**
 * @file internal.h
 * @author Nicolai Frigaard
 * @brief Internal ConsoleFW header file.
 * 
 * This file is the internal header file containing all internal API
 * in CFW. This header file is used by the internal code base.
 * 
 * @copyright Copyright (c) 2020
 */

#ifndef __cfw_internal_h__
#define __cfw_internal_h__

#if _MSC_VER > 1000
#pragma once
#endif

#include "CFW/cfw.h"

#define CFW_SWAP_POINTERS(x, y) \
    {                           \
        void *temp;             \
        temp = x;               \
        x = y;                  \
        y = temp;               \
    }

#define CFW_SWAP_VALUES(x, y)   \
    {                           \
        int temp = x;           \
        x = y;                  \
        y = temp;               \
    }

#define CFW_REQUIRE_INIT()                              \
    if (!__cfw.initialized)                             \
    {                                                   \
        _cfw_input_error(CFW_NOT_INITIALIZED, NULL);    \
        return;                                         \
    }

#define CFW_REQUIRE_INIT_OR_RETURN(x)                   \
    if (!__cfw.initialized)                             \
    {                                                   \
        _cfw_input_error(CFW_NOT_INITIALIZED, NULL);    \
        return x;                                       \
    }

#define CFW_REQUIRE_FEATURE_ENABLED(x)                  \
    if (!(__cfw.enabled_features & x))                  \
    {                                                   \
        _cfw_input_error(CFW_NOT_ENABLED, NULL);        \
        return;                                         \
    }

#define substr(dest, res, start, end)                   \
    memcpy(dest, &res[start], end); dest[end] = '\0';

#define max(x,y) (((x) >= (y)) ? (x) : (y))
#define min(x,y) (((x) <= (y)) ? (x) : (y))

typedef struct __cfx_library    __cfx_library;
typedef struct __cfw_region     __cfw_region;

struct __cfw_region
{
    int x;
    int y;
    int width;
    int height;

    // Pointer to next region in list
    __cfw_region *next;
};


struct __cfx_library
{
    cfw__bool       initialized;

    int             width;
    int             height;

    int             enabled_features;

    int             polygon_mode;

    int             foreground_color;
    int             background_color;

    __cfw_region    *region_head;
};

extern __cfx_library __cfw;

// ------------------------------------------------------------------
// |                        CFW internal API                        |
// ------------------------------------------------------------------

#if defined(__GNUC__)
void _cfw_input_error(int errorcode, const char *fmt, ...)
    __attribute__((format(printf, 2, 3)));
#else
void _cfw_input_error(int errorcode, const char *fmt, ...);
#endif

// ------------------------------------------------------------------
// |                        CFW platform API                        |
// ------------------------------------------------------------------

cfw__bool   _cfw_platform_init(void);
void        _cfw_platform_terminate(void);
void        _cfw_platform_refresh(void);
cfw__bool   _cfw_platform_is_feature_supported(int feature);
void        _cfw_platform_enable(int feature);
void        _cfw_platform_get_console_size(int *width, int *height);

void        _cfw_platform_clear(void);
void        _cfw_platform_set_color(int fg_color, int bg_color);
void        _cfw_platform_unset_color(int fg_color, int bg_color);
void        _cfw_platform_draw_char(int x, int y, char c);
void        _cfw_platform_draw_str(int x, int y, const char* str);

#endif /* __cfw_internal_h__ */
