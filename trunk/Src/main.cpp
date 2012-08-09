
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#if defined(_MSC_VER)
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif

#include "audio.h"
#include "display.h"
#include "input.h"
#include "gameplay.h"
#include "minimap.h"
#include "doom/WadFile.h"
#include "doom/lumps/PatchLump.h"
#include "GameStates/MainMenu.h"

LPDIRECT3D9 g_d3d;
LPDIRECT3DDEVICE9 g_d3ddev;
HWND hWnd;

long Init();

// Rendering parameters
doom::WadFile *g_doomwad = NULL;

// Keyboard action are taken into account every KEYBOARD_RATE_MS milliseconds
#define KEYBOARD_RATE_MS 5

int HandleInput();
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if (GameState::m_currentState != NULL)
		return GameState::m_currentState->HandleMessage(hWnd, msg, wParam, lParam);
	else
	    return DefWindowProc( hWnd, msg, wParam, lParam );
}


// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
	int x = 1;
	++x = x++;
    Init();
	
    MSG msg;
    while(TRUE)
    {
        while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if(msg.message == WM_QUIT)
            break;
		GameState::m_currentState->Render();
    }
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
								WS_OVERLAPPEDWINDOW, 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT,
								GetDesktopWindow(), NULL, wc.hInstance, NULL );
	ShowWindow(hWnd, 1);

    g_d3d = Direct3DCreate9(D3D_SDK_VERSION);

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
    d3dpp.BackBufferWidth = SCREEN_WIDTH;
    d3dpp.BackBufferHeight = SCREEN_HEIGHT;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    g_d3d->CreateDevice(D3DADAPTER_DEFAULT,
                      D3DDEVTYPE_HAL,
                      hWnd,
                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                      &d3dpp,
                      &g_d3ddev);

    g_d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);    // turn off the 3D lighting
    g_d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);    // turn off culling
    g_d3ddev->SetRenderState(D3DRS_ZENABLE, FALSE);    // turn on the z-buffer
	g_d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	GameState::m_currentState = new MainMenu();

	atexit(SDL_Quit);
	return 0;
}

