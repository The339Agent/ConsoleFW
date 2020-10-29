#ifndef __cfw_internal_h__
#define __cfw_internal_h__

#if _MSC_VER > 1000
#pragma once
#endif

#include "CFW/cfw.h"

typedef struct __cfx_library __cfx_library;

struct __cfx_library
{
    cfw__bool initialized;
};

extern __cfx_library __cfx;

// ------------------------------------------------------------------
// |                        CFW platform API                        |
// ------------------------------------------------------------------

cfw__bool _cfw_platform_init(void);
void _cfw_platform_terminate(void);

#endif /* __cfw_internal_h__ */