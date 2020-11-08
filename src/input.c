/**
 * @file input.c
 * @author Nicolai Frigaard
 * @brief Implementation of public input API.
 * 
 * The definition of API calls used for input from the console are
 * found in this file.
 * 
 * @copyright Copyright (c) 2020
 */

#include <stdlib.h>
#include <stdio.h>

#include "internal.h"

void _cfw_poll_input(void)
{
    if (__cfw.callbacks.char_callback)
    {
        int ch;
        for (;;)
        {
            if ((ch = _cfw_platform_get_char_no_halt()) == CFW_NO_KEY)
                break; // No more keys are pressed
            else
                __cfw.callbacks.char_callback(ch);
        }
    }
}

// ------------------------------------------------------------------
// |                         CFW PUBLIC API                         |
// ------------------------------------------------------------------

CFWAPI cfw__charfun cfw_set_char_callback(cfw__charfun cbfun)
{
    CFW_REQUIRE_INIT_OR_RETURN(NULL);
    CFW_SWAP_POINTERS(__cfw.callbacks.char_callback, cbfun);
    return cbfun;
}

CFWAPI int cfw_get_char(void)
{
    CFW_REQUIRE_INIT_OR_RETURN(CFW_NO_KEY);
    return _cfw_platform_get_char();
}

CFWAPI int cfw_get_pressed_char(void)
{
    CFW_REQUIRE_INIT_OR_RETURN(CFW_NO_KEY);

    // Get the currently pressed char
    int key = _cfw_platform_get_char_no_halt();

    // Since calling char_no_halt in some cases removes the char from
    // the char queue, it will not be detected in poll_input. To make
    // sure all char inputs are given to the char callback function,
    // this key has to trigger the event to.
    if (__cfw.callbacks.char_callback != NULL && key != CFW_NO_KEY)
        __cfw.callbacks.char_callback(key);

    // Return the char
    return key;
}