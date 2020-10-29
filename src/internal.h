#ifndef __cfw_internal_h__
#define __cfw_internal_h__

#if _MSC_VER > 1000
#pragma once
#endif

#include "CFW/cfw.h"

#define CFW_REQUIRE_INIT()      \
    if (!__cfw.initialized)     \
    {                           \
        return;                 \
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

cfw__bool _cfw_platform_init(void);
void _cfw_platform_terminate(void);
void _cfw_platform_refresh(void);

void _cfw_platform_clear(void);
void _cfw_platform_draw_str(int x, int y, const char* str);

#endif /* __cfw_internal_h__ */