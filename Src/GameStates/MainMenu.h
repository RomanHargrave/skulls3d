
#ifndef SK_MAINMENU
#define SK_MAINMENU

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "GameState.h"

class MainMenu : public GameState
{
	LPDIRECT3DVERTEXBUFFER9 m_vbuffer;   // the pointer to the vertex buffer
	LPDIRECT3DVERTEXBUFFER9 m_vbuffer2;   // the pointer to the vertex buffer
	LPDIRECT3DINDEXBUFFER9 m_ibuffer;    // the pointer to the index buffer
	LPDIRECT3DTEXTURE9 m_texture;

	LPDIRECT3DVERTEXBUFFER9 m_ngvb;   // the pointer to the vertex buffer
	LPDIRECT3DTEXTURE9 m_newGame;
	
public:
	MainMenu();

	virtual void Render();
	virtual LRESULT WINAPI HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

#endif // SK_MAINMENU