#include <windows.h>
#include <SDL.h>
#include <d3d9.h>
#include <d3dx9.h>


#define VERTEXFORMAT (D3DFVF_XYZ | D3DFVF_DIFFUSE)

IDirect3DVertexBuffer9 *vBuffer;
IDirect3DDevice9* dDev;
IDirect3D9* dObj;

struct typeVertex{
	float x,y,z;
	D3DCOLOR color;	
};

typeVertex* vertex;

void DxTest(){
	
	SDL_SysWMinfo SysInfo;
	SDL_VERSION(&SysInfo.version);

	HWND wndHandle = SysInfo.window;

	dObj = Direct3DCreate9(D3D_SDK_VERSION);
	
	D3DDISPLAYMODE  dMode;	
	dMode.Width = 800;
	dMode.Height = 600;

	dMode.RefreshRate = 0;
	dMode.Format = D3DFMT_A8R8G8B8;

	D3DPRESENT_PARAMETERS dParam;
	ZeroMemory(&dParam, sizeof(D3DPRESENT_PARAMETERS));
	dParam.Windowed = TRUE;
	dParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
	dParam.BackBufferFormat = D3DFMT_A8R8G8B8;
	dParam.BackBufferWidth = 800;
	dParam.BackBufferHeight = 600;
	dParam.EnableAutoDepthStencil = TRUE;
	dParam.AutoDepthStencilFormat = D3DFMT_D16;

	dObj->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, wndHandle, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &dParam, &dDev);
	vBuffer = NULL;

	int qty = 1;
	vertex = new typeVertex[qty];

	vertex[1].x = 0;
	vertex[1].y = 0;
	vertex[1].z = 0;
	vertex[1].color = D3DCOLOR_ARGB(255,255,255,255);;
	
	dDev->CreateVertexBuffer( sizeof(typeVertex)*qty, 0,VERTEXFORMAT, D3DPOOL_MANAGED , &vBuffer, NULL);
	BYTE *lPointer;

	vBuffer->Lock(0, 0, (void**)&lPointer, 0);
	memcpy(lPointer, &vertex[0], sizeof(typeVertex) * qty);
	vBuffer->Unlock();

	dDev->SetStreamSource(0, vBuffer, 0, sizeof(typeVertex));
	dDev->SetFVF(VERTEXFORMAT);

	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	dDev->SetTransform(D3DTS_WORLD, &mat);

	D3DXMatrixIdentity(&mat);
	D3DXMatrixLookAtLH(&mat, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(-50, 0, 0), &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	dDev->SetTransform(D3DTS_VIEW, &mat);

	D3DXMatrixIdentity(&mat);
	D3DXMatrixPerspectiveFovLH(&mat, D3DX_PI/6, ((float)800/(float)600), 128.0f, 1800.0f);
	dDev->SetTransform(D3DTS_PROJECTION, &mat);	

	dDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	dDev->SetRenderState(D3DRS_SRCBLEND ,D3DBLEND_SRCALPHA);
	dDev->SetRenderState(D3DRS_DESTBLEND ,D3DBLEND_INVSRCALPHA );
	dDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	dDev->SetRenderState(D3DRS_ALPHAREF, 0x08);
	dDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	dDev->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	dDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);


	ShowWindow(wndHandle, SW_SHOW);
	UpdateWindow(wndHandle);
	
	dDev->Clear(0, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, D3DCOLOR_RGBA(0,0,0,0), 1.0f, 0);
	dDev->BeginScene();
	dDev->DrawPrimitive(D3DPT_POINTLIST, 1, 1);
	dDev->EndScene();

	if (vBuffer != NULL)
		vBuffer->Release();
	if (dDev != NULL)
		dDev->Release();
	if (dObj != NULL)
		dObj->Release();	
}
