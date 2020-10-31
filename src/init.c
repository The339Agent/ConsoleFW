/**
 * @file init.c
 * @author Nicolai Frigaard
 * @brief Implementation of global generic public API functionality.
 * 
 * This file contains the definition of the public API of CFW that
 * controls the entirety of the CFW library.
 * 
 * @copyright Copyright (c) 2020
 */

#include <memory.h>

#include "internal.h"

// Global state variables
__cfx_library __cfw = { CFW_FALSE };

// ------------------------------------------------------------------
// |                         CFW PUBLIC API                         |
// ------------------------------------------------------------------

CFWAPI cfw__bool cfw_init(void)
{
    if (__cfw.initialized)
        return CFW_TRUE;

    // The first time cfx_init() is called, only the initialized
    // variable is set. To avoid bugs, the entire struct must be set
    // to 0 to empty the data that was there before.
    memset(&__cfw, 0, sizeof(__cfw));
    
    if (_cfw_platform_init() == CFW_FALSE)
    {
        cfw_terminate();
        return CFW_FALSE;
    }

    // Set default values
    __cfw.polygon_mode = CFW_FILL;

    // CFW is now initialized
    __cfw.initialized = CFW_TRUE;

    return CFW_TRUE;
}

CFWAPI void cfw_terminate(void)
{
    if (!__cfw.initialized)
        return;

    _cfw_platform_terminate();

    // The memset that sets the entire __cfw struct to 0 also sets
    // the initialized variable to false, but it's more clear this
    // way.
    __cfw.initialized = CFW_FALSE;

    // Clear the global data
    memset(&__cfw, 0, sizeof(__cfw));
}

CFWAPI void cfw_refresh(void)
{
    CFW_REQUIRE_INIT();
    _cfw_platform_refresh();
}

CFWAPI cfw__bool cfw_is_feature_supported(int feature)
{
    CFW_REQUIRE_INIT_OR_RETURN(CFW_FALSE);
    return _cfw_platform_is_feature_supported(feature);
}

CFWAPI void cfw_get_console_size(int *width, int *height)
{
    CFW_REQUIRE_INIT();

    // Get size of console
    int _width, _height;
    _cfw_platform_get_console_size(&_width, &_height);

    // Save the size data in the given pointers
    if (width  != NULL) *width  = _width;
    if (height != NULL) *height = _height;
}