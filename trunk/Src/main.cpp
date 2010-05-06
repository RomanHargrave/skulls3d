
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <d3d9.h>

#if defined(_MSC_VER)
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif

#include "audio.h"
#include "display.h"
#include "input.h"
#include "mainmenu.h"
#include "gameplay.h"
#include "minimap.h"
#include "doom/WadFile.h"

IDirect3D9 *g_pD3D;
IDirect3DDevice9 *g_pd3dDevice;
IDirect3DVertexBuffer9 *g_pVB;
HWND hWnd;
long Init();

// Rendering parameters
doom::WadFile *g_doomwad = NULL;
SDL_Surface *g_screen;

// Keyboard action are taken into account every KEYBOARD_RATE_MS milliseconds
#define KEYBOARD_RATE_MS 5

int HandleInput();

void Render()
{
	// Clear the back buffer to a blue color
	g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );

	// Begin the scene
	g_pd3dDevice->BeginScene();

	g_pd3dDevice->SetStreamSource( 0, g_pVB, 0, sizeof(CUSTOMVERTEX) );
	g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
	g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 1 );

	// End the scene
	g_pd3dDevice->EndScene();

	g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}

LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
            PostQuitMessage( 0 );
            return 0;

        case WM_PAINT:
            Render();
            ValidateRect( hWnd, NULL );
            return 0;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}


int main(int argc, char *argv[])
{
    Init();

	//ShowMainMenu();
	doom::LevelLump *level = g_doomwad->GetLevel(0);
	PlayLevel(level);
	
	/*
	while (1)
	{
		RefreshDebugDisplay(screen);
		if (int a=HandleInput() < 0)
			return -a;
	}
	*/

	CloseAudio();
	return 0;
}


long Init()
{
	InitKeyboardInput();
	InitAudio();
	
	g_doomwad = new doom::WadFile("doom.wad");
	if (g_doomwad == NULL)
		return -1;
	if (g_doomwad->Open() != 0)
		return -1;
	//if (g_doomwad->Load() != true)
	//	return -1;
	g_doomwad->Load();

	// Register the window class.
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, 
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		L"Direct3D Tutorial", NULL };

	RegisterClassEx( &wc );

	// Create the application's window.
	hWnd = CreateWindow( L"Direct3D Tutorial", L"Direct3D Tutorial 01: CreateDevice", 
								WS_OVERLAPPEDWINDOW, 100, 100, 300, 300,
								GetDesktopWindow(), NULL, wc.hInstance, NULL );

	ShowWindow(hWnd, 1);

	if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		return E_FAIL;

	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                  D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                  &d3dpp, &g_pd3dDevice ) ) )
		return E_FAIL;

	if( FAILED( g_pd3dDevice->CreateVertexBuffer( 3000*sizeof(CUSTOMVERTEX),
         0 /*Usage*/, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_pVB, NULL ) ) )
		return E_FAIL;

	atexit(SDL_Quit);
	return 0;
}


