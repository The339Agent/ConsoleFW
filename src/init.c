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
#include <stdarg.h>
#include <stdio.h>

#include "internal.h"

// Global state variables
__cfx_library __cfw = { CFW_FALSE };

// Global error callback
cfw__errorfun __error_callback;

// ------------------------------------------------------------------
// |                        CFW internal API                        |
// ------------------------------------------------------------------

void _cfw_input_error(int errorcode, const char *fmt, ...)
{
    char message[1024]; // Max size of the final message is 1024

    if (fmt)
    {
        va_list va;

        // Set the message to be the formatted string
        va_start(va, fmt);
        vsnprintf(message, sizeof(message), fmt, va);
        va_end(va);

        // Set last char of the message to be the nulltermination
        // character
        message[sizeof(message) - 1] = '\0';
    }
    else
    {
        // Automatically generate message from errorcode
        switch (errorcode)
        {
        case CFW_NOT_INITIALIZED:
            strcpy(message, "CFW is not initialized");
            break;
        case CFW_INVALID_VALUE:
            strcpy(message, "Given value is invalid");
            break;
        case CFW_NOT_ENABLED:
            strcpy(message, "CFW feature is not enabled");
            break;
        default:
            strcpy(message, "Unknown error");
            break;
        }
    }
    
    if (__error_callback)
        __error_callback(errorcode, message);
}

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

    // Cache console size
    cfw_get_console_size(&__cfw.width, &__cfw.height);

    // Set default values
    __cfw.polygon_mode = CFW_FILL;
    __cfw.foreground_color = -1;
    __cfw.background_color = -1;

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

CFWAPI cfw__errorfun cfw_set_error_callback(cfw__errorfun cbfun)
{
    CFW_SWAP_POINTERS(__error_callback, cbfun);
    return cbfun;
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

CFWAPI void cfw_enable(int feature)
{
    CFW_REQUIRE_INIT();
    _cfw_platform_enable(feature);
    __cfw.enabled_features |= feature;
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

    // ...and just take the opportunity to update the cached size
    // variables... shh...
    __cfw.width  = _width;
    __cfw.height = _height;
}
