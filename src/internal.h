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

#define CFW_REQUIRE_INIT()              \
    if (!__cfw.initialized)             \
    {                                   \
        return;                         \
    }

#define CFW_REQUIRE_INIT_OR_RETURN(x)   \
    if (!__cfw.initialized)             \
    {                                   \
        return x;                       \
    }

typedef struct __cfx_library __cfx_library;

struct __cfx_library
{
    cfw__bool initialized;
};

extern __cfx_library __cfw;

// ------------------------------------------------------------------
// |                        CFW platform API                        |
// ------------------------------------------------------------------

cfw__bool   _cfw_platform_init(void);
void        _cfw_platform_terminate(void);
void        _cfw_platform_refresh(void);
cfw__bool   _cfw_platform_is_feature_supported(int feature);
void        _cfw_platform_get_console_size(int *width, int *height);

void        _cfw_platform_clear(void);
void        _cfw_platform_draw_char(int x, int y, char c);
void        _cfw_platform_draw_str(int x, int y, const char* str);

#endif /* __cfw_internal_h__ */