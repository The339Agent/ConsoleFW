#ifndef __cfw_ncurses_internal_h__
#define __cfw_ncurses_internal_h__

#if _MSC_VER > 1000
#pragma once
#endif

// ------------------------------------------------------------------
// |                   CFW internal platform API                    |
// ------------------------------------------------------------------

int         _cfw_platform_ncurses_colornum(int foreground, int background);
cfw__bool   _cfw_platform_ncurses_is_bold(int color);

#endif /* __cfw_ncurses_internal_h__ */