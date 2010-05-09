
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
#include "mainmenu.h"
#include "gameplay.h"
#include "minimap.h"
#include "doom/WadFile.h"

LPDIRECT3D9 g_d3d;
LPDIRECT3DDEVICE9 g_d3ddev;
HWND hWnd;

static LPDIRECT3DVERTEXBUFFER9 g_vbuffer = NULL;    // the pointer to the vertex buffer
static LPDIRECT3DINDEXBUFFER9 g_ibuffer = NULL;    // the pointer to the index buffer
long Init();
void init_graphics(void);

// Rendering parameters
doom::WadFile *g_doomwad = NULL;

// Keyboard action are taken into account every KEYBOARD_RATE_MS milliseconds
#define KEYBOARD_RATE_MS 5

int HandleInput();

void Render()
{
    g_d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    g_d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    g_d3ddev->BeginScene();

    g_d3ddev->SetFVF(D3DFVF_CUSTOMVERTEX);

    // set the view transform
    D3DXMATRIX matView;    // the view transform matrix
    D3DXMatrixLookAtLH(&matView,
    &D3DXVECTOR3 (0.0f, 8.0f, 25.0f),    // the camera position
    &D3DXVECTOR3 (0.0f, 0.0f, 0.0f),      // the look-at position
    &D3DXVECTOR3 (0.0f, 1.0f, 0.0f));    // the up direction
    g_d3ddev->SetTransform(D3DTS_VIEW, &matView);    // set the view transform to matView 

    // set the projection transform
    D3DXMATRIX matProjection;    // the projection transform matrix
    D3DXMatrixPerspectiveFovLH(&matProjection,
                               D3DXToRadian(45),    // the horizontal field of view
                               (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
                               1.0f,   // the near view-plane
                               100.0f);    // the far view-plane
    g_d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection); // set the projection

    // set the world transform
    static float index = 0.0f; index+=0.001f; // an ever-increasing float value
    D3DXMATRIX matRotateY;    // a matrix to store the rotation for each triangle
    D3DXMatrixRotationY(&matRotateY, index);    // the rotation matrix
    g_d3ddev->SetTransform(D3DTS_WORLD, &(matRotateY));    // set the world transform

     // select the vertex buffer to display
    g_d3ddev->SetStreamSource(0, g_vbuffer, 0, sizeof(CUSTOMVERTEX));
    g_d3ddev->SetIndices(g_ibuffer);

    // draw the Hypercraft
    g_d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 10, 0, 6);

    g_d3ddev->EndScene(); 

    g_d3ddev->Present(NULL, NULL, NULL, NULL);
}

LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
            PostQuitMessage( 0 );
            return 0;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}


// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    Init();

	//ShowMainMenu();
	doom::LevelLump *level = g_doomwad->GetLevel(0);
	
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

        Render();
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
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
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

    init_graphics();

    g_d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);    // turn off the 3D lighting
    g_d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);    // turn off culling
    g_d3ddev->SetRenderState(D3DRS_ZENABLE, FALSE);    // turn on the z-buffer


	atexit(SDL_Quit);
	return 0;
}



// this is the function that puts the 3D models into video RAM
void init_graphics(void)
{
    // create the vertices using the CUSTOMVERTEX
    CUSTOMVERTEX vertices[] =
    {
        { -3.0f, -3.0f, 0.0f, 0.0f, 200.0f },
        {  3.0f, -3.0f, 0.0f, 300.0f, 200.0f },
        { -3.0f,  3.0f, 0.0f, 0.0f, 0.0f },
        {  3.0f,  3.0f, 0.0f, 300.0f, 0.0f },
    };

    // create a vertex buffer interface called v_buffer
    g_d3ddev->CreateVertexBuffer(4*sizeof(CUSTOMVERTEX),
                               0,
                               D3DFVF_CUSTOMVERTEX,
                               D3DPOOL_MANAGED,
                               &g_vbuffer,
                               NULL);

    VOID* pVoid;    // a void pointer

    // lock v_buffer and load the vertices into it
    g_vbuffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, vertices, sizeof(vertices));
    g_vbuffer->Unlock();

    // create the indices using an int array
    short indices[] =
    {
        0, 1, 3,
        0, 3, 2,
    };

    // create a index buffer interface called i_buffer
    g_d3ddev->CreateIndexBuffer(6*sizeof(short),
                              0,
                              D3DFMT_INDEX16,
                              D3DPOOL_MANAGED,
                              &g_ibuffer,
                              NULL);

    // lock i_buffer and load the indices into it
    g_ibuffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, indices, sizeof(indices));
    g_ibuffer->Unlock(); 
}
