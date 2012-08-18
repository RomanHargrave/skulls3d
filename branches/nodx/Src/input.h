
#ifndef SK_INPUT
#define SK_INPUT

#if defined(_MSC_VER)
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif

// Keyboard state
extern char g_keys[256];
extern float g_mouse_x;
extern float g_mouse_y;

void InitKeyboardInput();
SDL_Event RefreshKeybState();

#define KEYBOARD_RATE_MS 5

#endif // SK_INPUT