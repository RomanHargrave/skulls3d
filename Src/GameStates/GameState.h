
#ifndef SK_GAMESTATE
#define SK_GAMESTATE

#include <windows.h>

class GameState
{
public:
	static GameState * m_currentState;

	virtual void Render() = 0;
	virtual LRESULT WINAPI HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
};

#endif // SK_GAMESTATE
