
#ifndef SK_INPUT
#define SK_INPUT

// Keyboard state
extern char g_keys[256];

void InitKeyboardInput();
int RefreshKeybState();

#endif // SK_INPUT