
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "main.h"
#include "MainMenu.h"
#include "InGame.h"
#include "doom/WadFile.h"
#include "doom/lumps/PatchLump.h"
#include "doom/lumps/LevelLump.h"

void WINAPI FillFunc(D3DXVECTOR4* pOut, CONST D3DXVECTOR2* pTexCoord, CONST D3DXVECTOR2* pTexelSize, LPVOID pData);


MainMenu::MainMenu()
{
    // create the vertices using the CUSTOMVERTEX
    CUSTOMVERTEX vertices[] =
    {
        { -4.0f, -3.0f, 0.0f, 0.0f, 1.0f },
        {  4.0f, -3.0f, 0.0f, 1.0f, 1.0f },
        { -4.0f,  3.0f, 0.0f, 0.0f, 0.0f },
        {  4.0f,  3.0f, 0.0f, 1.0f, 0.0f },
    };
    CUSTOMVERTEX vertices2[] =
    {
        { -2.0f, -1.0f, 0.0f, 0.0f, 1.0f },
        {  2.0f, -1.0f, 0.0f, 1.0f, 1.0f },
        { -2.0f,  1.0f, 0.0f, 0.0f, 0.0f },
        {  2.0f,  1.0f, 0.0f, 1.0f, 0.0f },
    };

    // create a vertex buffer interface called v_buffer
    g_d3ddev->CreateVertexBuffer(4*sizeof(CUSTOMVERTEX),
                               0,
                               D3DFVF_CUSTOMVERTEX,
                               D3DPOOL_MANAGED,
                               &m_vbuffer,
                               NULL);

    // create a vertex buffer interface called v_buffer
    g_d3ddev->CreateVertexBuffer(4*sizeof(CUSTOMVERTEX),
                               0,
                               D3DFVF_CUSTOMVERTEX,
                               D3DPOOL_MANAGED,
                               &m_vbuffer2,
                               NULL);

    VOID* pVoid;    // a void pointer

    // lock v_buffer and load the vertices into it
    m_vbuffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, vertices, sizeof(vertices));
    m_vbuffer->Unlock();
    m_vbuffer2->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, vertices2, sizeof(vertices2));
    m_vbuffer2->Unlock();

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
							  &m_ibuffer,
                              NULL);

    // lock i_buffer and load the indices into it
    m_ibuffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, indices, sizeof(indices));
    m_ibuffer->Unlock(); 

	doom::PatchLump * titleBackg = g_doomwad->GetLump((doom::PatchLump*)g_doomwad->Get("TITLEPIC"));
	titleBackg->Load();
	D3DXCreateTexture(g_d3ddev, titleBackg->m_w, titleBackg->m_h,
		1, D3DUSAGE_RENDERTARGET, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, &m_texture);
	D3DXFillTexture(m_texture, FillFunc, titleBackg);

	doom::PatchLump * newGame = g_doomwad->GetLump((doom::PatchLump*)g_doomwad->Get("M_NGAME"));
	newGame->Load();
	D3DXCreateTexture(g_d3ddev, newGame->m_w, newGame->m_h,
		1, D3DUSAGE_RENDERTARGET, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, &m_newGame);
	D3DXFillTexture(m_newGame, FillFunc, newGame);
}

void MainMenu::Render()
{
    g_d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	g_d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

    g_d3ddev->BeginScene();

    g_d3ddev->SetFVF(D3DFVF_CUSTOMVERTEX);

    // set the view transform
    D3DXMATRIX matView;    // the view transform matrix
    D3DXMatrixLookAtLH(&matView,
    &D3DXVECTOR3 (0.0f, 0.0f, -7.24f),    // the camera position
    &D3DXVECTOR3 (0.0f, 0.0f, 0.0f),      // the look-at position
    &D3DXVECTOR3 (0.0f, 1.0f, 0.0f));    // the up direction
    g_d3ddev->SetTransform(D3DTS_VIEW, &matView);    // set the view transform to matView 

    // set the projection transform
    D3DXMATRIX matProjection;    // the projection transform matrix
    D3DXMatrixPerspectiveFovLH(&matProjection,
                               D3DXToRadian(45),    // the horizontal field of view
                               (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
                               0.01f,   // the near view-plane
                               100.0f);    // the far view-plane
    g_d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection); // set the projection

    // set the world transform
    //static float index = 0.0f; index+=0.01f; // an ever-increasing float value
    //D3DXMATRIX matRotateY;    // a matrix to store the rotation for each triangle
    //D3DXMatrixRotationY(&matRotateY, index);    // the rotation matrix
    //g_d3ddev->SetTransform(D3DTS_WORLD, &(matRotateY));    // set the world transform

    // select the vertex buffer to display
    g_d3ddev->SetStreamSource(0, m_vbuffer, 0, sizeof(CUSTOMVERTEX));
    g_d3ddev->SetIndices(m_ibuffer);

    // draw the Hypercraft
    g_d3ddev->SetTexture( 0, m_texture );
    g_d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 10, 0, 6);

    g_d3ddev->SetStreamSource(0, m_vbuffer2, 0, sizeof(CUSTOMVERTEX));
	g_d3ddev->SetTexture( 0, m_newGame );
	g_d3ddev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE /*D3DTA_DIFFUSE*/);
	g_d3ddev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	g_d3ddev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_BLENDDIFFUSEALPHA);
	g_d3ddev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_d3ddev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	g_d3ddev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_BLENDTEXTUREALPHA);
    g_d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 10, 0, 6);

    g_d3ddev->EndScene(); 
    g_d3ddev->Present(NULL, NULL, NULL, NULL);
}

LRESULT WINAPI MainMenu::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch( msg )
    {
		case WM_KEYDOWN:
            switch (wParam) 
            { 
				case VK_RETURN: 
					doom::LevelLump *level = g_doomwad->GetLevel(0);
					level->Load();
					GameState::m_currentState = new InGame(g_doomwad->GetLevel(0));
					break;
			}
			break;

        case WM_DESTROY:
            PostQuitMessage( 0 );
            return 0;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}

void WINAPI FillFunc(D3DXVECTOR4* pOut, CONST D3DXVECTOR2* pTexCoord, CONST D3DXVECTOR2* pTexelSize, LPVOID pData)
{
	doom::PatchLump * gs_titleBackg = (doom::PatchLump *) pData;
	int coord = gs_titleBackg->m_w*((int)(pTexCoord->y*gs_titleBackg->m_h)) + ((int)(pTexCoord->x*gs_titleBackg->m_w));
	pOut->w = 0.9f;//(((gs_titleBackg->m_bitmap[coord] >> 24) & 0xFF) / 255.0f);
	pOut->x = ((gs_titleBackg->m_bitmap[coord] >> 16) & 0xFF) / 255.0f;
	pOut->y = ((gs_titleBackg->m_bitmap[coord] >>  8) & 0xFF) / 255.0f;
	pOut->z = ((gs_titleBackg->m_bitmap[coord]      ) & 0xFF) / 255.0f;
}
