
#ifndef SK_MAIN
#define SK_MAIN

#include <windows.h>
#include <d3d9.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

extern LPDIRECT3D9 g_d3d;
extern LPDIRECT3DDEVICE9 g_d3ddev;
extern HWND hWnd;
typedef struct 
{
	FLOAT x, y, z;
	FLOAT u, v;
}CUSTOMVERTEX;
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_TEX0)

#endif // SK_MAIN
