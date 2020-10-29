#include "internal.h"

// ------------------------------------------------------------------
// |                         CFW PUBLIC API                         |
// ------------------------------------------------------------------

CFWAPI void cfw_clear(void)
{
    CFW_REQUIRE_INIT();
    _cfw_platform_clear();
}

CFWAPI void cfw_draw_str(int x, int y, const char* str)
{
    CFW_REQUIRE_INIT();
    _cfw_platform_draw_str(x, y, str);
}