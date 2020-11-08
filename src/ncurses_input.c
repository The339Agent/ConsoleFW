/**
 * @file ncurses_input.c
 * @author your name (you@domain.com)
 * @brief The ncurses implementation of platform-specific input code.
 * 
 * This file contains the ncurses implementation of the
 * platform-specific input code.
 * 
 * @copyright Copyright (c) 2020
 */

#include <ncurses.h>

#include "internal.h"

int translate_function_key(int key)
{
    switch (key) 
    {
    case KEY_DOWN:  return CFW_KEY_DOWN;
    case KEY_UP:    return CFW_KEY_UP;
    case KEY_LEFT:  return CFW_KEY_LEFT;
    case KEY_RIGHT: return CFW_KEY_RIGHT;

    case KEY_HOME:      return CFW_KEY_HOME;
    case KEY_BACKSPACE: return CFW_KEY_BACKSPACE;
    case KEY_DC:        return CFW_KEY_DELETE;
    case KEY_IC:        return CFW_KEY_INSERT;

    case '\n':
    case '\r':
    case KEY_ENTER:     return CFW_KEY_ENTER;

    case KEY_F(1):  return CFW_KEY_F1;
    case KEY_F(2):  return CFW_KEY_F2;
    case KEY_F(3):  return CFW_KEY_F3;
    case KEY_F(4):  return CFW_KEY_F4;
    case KEY_F(5):  return CFW_KEY_F5;
    case KEY_F(6):  return CFW_KEY_F6;
    case KEY_F(7):  return CFW_KEY_F7;
    case KEY_F(8):  return CFW_KEY_F8;
    case KEY_F(9):  return CFW_KEY_F9;
    case KEY_F(10): return CFW_KEY_F10;
    case KEY_F(11): return CFW_KEY_F11;
    case KEY_F(12): return CFW_KEY_F12;
    case KEY_F(13): return CFW_KEY_F13;
    case KEY_F(14): return CFW_KEY_F14;
    case KEY_F(15): return CFW_KEY_F15;
    case KEY_F(16): return CFW_KEY_F16;
    case KEY_F(17): return CFW_KEY_F17;
    case KEY_F(18): return CFW_KEY_F18;
    case KEY_F(19): return CFW_KEY_F19;
    case KEY_F(20): return CFW_KEY_F20;
    case KEY_F(21): return CFW_KEY_F21;
    case KEY_F(22): return CFW_KEY_F22;
    case KEY_F(23): return CFW_KEY_F23;
    case KEY_F(24): return CFW_KEY_F24;
    case KEY_F(25): return CFW_KEY_F25;

    // If the key isn't a function key, assume it's a printable key,
    // and return it un-modified.
    default:        return key;
    }
}

// ------------------------------------------------------------------
// |                   CFW internal platform API                    |
// ------------------------------------------------------------------

int _cfw_platform_get_char(void)
{
    nodelay(stdscr, FALSE); // Enable halting
    return translate_function_key(getch());
}

int _cfw_platform_get_char_no_halt(void)
{
    nodelay(stdscr, TRUE); // Disable halting
    int ch = getch();

    if (ch != ERR)
        return translate_function_key(ch);
    else 
        return CFW_NO_KEY;
}