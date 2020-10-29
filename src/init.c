#include <memory.h>

#include "internal.h"

// Global state variables
__cfx_library __cfx = { CFW_FALSE };

// ------------------------------------------------------------------
// |                         CFW PUBLIC API                         |
// ------------------------------------------------------------------

CFWAPI cfw__bool cfw_init(void)
{
    if (__cfx.initialized)
        return CFW_TRUE;

    // The first time cfx_init() is called, only the initialized
    // variable is set. To avoid bugs, the entire struct must be set
    // to 0 to empty the data that was there before.
    memset(&__cfx, 0, sizeof(__cfx));
    
    if (_cfw_platform_init() == CFW_FALSE)
    {
        cfw_terminate();
        return CFW_FALSE;
    }

    return CFW_TRUE;
}

CFWAPI void cfw_terminate(void)
{
    if (!__cfx.initialized)
        return;

    _cfw_platform_terminate();

    // The memset that sets the entire __cfx struct to 0 also sets
    // the initialized variable to false, but it's more clear this
    // way.
    __cfx.initialized = CFW_FALSE;

    // Clear the global data
    memset(&__cfx, 0, sizeof(__cfx));

}