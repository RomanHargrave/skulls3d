
#ifndef SK_MAIN
#define SK_MAIN

#include <windows.h>
#include <d3d9.h>

extern IDirect3D9 *g_pD3D;
extern IDirect3DDevice9 *g_pd3dDevice;
extern IDirect3DVertexBuffer9 *g_pVB;
extern HWND hWnd;
typedef struct 
{
    FLOAT x, y, z, rhw; // The transformed position for the vertex.
    DWORD color;        // The vertex color.
}CUSTOMVERTEX;
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

#endif // SK_MAIN
