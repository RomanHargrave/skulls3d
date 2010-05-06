
#include <windows.h>
#include <math.h>
#include <stdlib.h>

#if defined(_MSC_VER)
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif

#include "gameplay.h"
#include "minimap.h"
#include "input.h"

#include "Matrix4x4.h"
#include "ViewPort.h"
#include "Scene.h"
#include "Renderer.h"
#include "Tex.h"
#include "Vec3f.h"
#include "RectangleTriangle.h"

#include "doom/Thing.h"
#include "doom/SideDef.h"
#include "doom/LineDef.h"
#include "doom/Vertex.h"
#include "doom/Sector.h"
#include "doom/Texture.h"
#include "doom/lumps/PatchLump.h"

#include "DataClasses/Cube.h"

// From display.cpp
extern unsigned int g_scr_w;
extern unsigned int g_scr_h;

// From main.cpp
extern SDL_Surface *g_screen;
LARGE_INTEGER g_renderingTime;
unsigned int g_frameCount = 0;

// From minimap.cpp
extern float g_zoom;
extern int g_gridZoom;

Scene* BuildScene(doom::LevelLump * level);
Mesh* CreateMesh(doom::Vertex v0, doom::Vertex v1, int low, int high, Tex *tex);
void VideoWorks();
int HandleGameplayInput();

static Scene *scene;
Camera *g_camera;
static ViewPort *viewport1;
static Renderer *renderer1 = NULL;
	
static float g_cameraxrotation;
static float g_camerayrotation;

void RenderGamePlay()
{
	// Clear the back buffer to a blue color
	g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );

	// Begin the scene
	g_pd3dDevice->BeginScene();

	for (int i=0 ; i<scene->m_meshCount ; i++)
	{
		VOID* pVertices;
		if( FAILED( g_pVB->Lock( 0, sizeof(CUSTOMVERTEX)*scene->m_meshes[i]->m_vertexcount, (void**)&pVertices, 0 ) ) )
			return;
		memcpy( pVertices, scene->m_meshes[i]->m_vb, sizeof(CUSTOMVERTEX)*scene->m_meshes[i]->m_vertexcount );
		g_pVB->Unlock();
		g_pd3dDevice->SetStreamSource( 0, g_pVB, 0, sizeof(CUSTOMVERTEX) );
		g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
		g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLEFAN, 0, 2 );
	}

	// End the scene
	g_pd3dDevice->EndScene();

	g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}

LRESULT WINAPI GamePlayProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
            PostQuitMessage( 0 );
            return 0;

        case WM_PAINT:
            RenderGamePlay();
            ValidateRect( hWnd, NULL );
            return 0;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}


void PlayLevel(doom::LevelLump * level)
{
	if (level == NULL)
		return;
	if (level->Load() != true)
		return;
	
	scene = BuildScene(level);

	g_camera = new Camera(g_scr_w/(1.0f*g_scr_h));
	viewport1 = new ViewPort(0, 0, g_scr_w, g_scr_h, g_screen);
	renderer1 = new Renderer(scene, g_camera, viewport1);
	g_camera->Translate(level->m_things[0]->m_x, 0.0f, level->m_things[0]->m_z);

	SetWindowLongW(hWnd, GWL_WNDPROC, (LONG)GamePlayProc);

	MSG msg; 
	while( GetMessage( &msg, NULL, 0, 0 ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	//ShowMinimap(level);
	/*
	while (1)
	{
		VideoWorks();
		SDL_Event event = RefreshKeybState();
		if (event.type == SDL_QUIT)
			exit(0);
		else if (event.type==SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_TAB)
				ShowMinimap(level);
			else if (event.key.keysym.sym == SDLK_ESCAPE)
				return;
		}
		HandleGameplayInput(event);
	}
	*/
	
	return;
}

Scene* BuildScene(doom::LevelLump * level)
{
	Scene *s = new Scene();
	unsigned int count;

	/*
	count = (unsigned int) level->m_things.size();
	for (unsigned int i=0 ; i<count ; i++)
	{
		doom::Thing * thingLump = level->m_things[i];

		Texture *texture = new Texture(thingLump->m_sprites[0]->m_bitmap,
									   thingLump->m_sprites[0]->m_w,
									   thingLump->m_sprites[0]->m_h);

		Cube *c = new Cube(texture->m_mipmaps[0].m_height*1.0f, texture);
		c->m_worldmatrix.Translate(thingLump->m_x, 0.0f, thingLump->m_z);
		s->AddMesh(c);
	}
	*/

	Tex *texture = new Tex(0x00FFFFFF);
	count = (unsigned int) level->m_sideDefs.size();
	for (unsigned int i=0 ; i<count ; i++)
	{
		doom::SideDef *sideDef = level->m_sideDefs[i];
		doom::Vertex *v0, *v1;
		doom::Sector *sector, *otherSector;
		sector = sideDef->m_sector;
		otherSector = NULL;
		if (sideDef == sideDef->m_lineDef->m_rightSideDef)
		{
			v0 = sideDef->m_lineDef->m_start_vtx;
			v1 = sideDef->m_lineDef->m_end_vtx;
			if (sideDef->m_lineDef->m_leftSideDef != NULL)
				otherSector = sideDef->m_lineDef->m_leftSideDef->m_sector;
		}
		else
		{
			v1 = sideDef->m_lineDef->m_start_vtx;
			v0 = sideDef->m_lineDef->m_end_vtx;
			if (sideDef->m_lineDef->m_rightSideDef != NULL)
				otherSector = sideDef->m_lineDef->m_rightSideDef->m_sector;
		}
		if (otherSector == NULL)
		{	
			Mesh *mesh = CreateMesh(*v0, *v1,
			                        sideDef->m_sector->m_floorHeight,
			                        sideDef->m_sector->m_ceilingHeight,
			                        new Tex(sideDef->m_middleTexture->m_bitmap, sideDef->m_middleTexture->m_w, sideDef->m_middleTexture->m_h)
			                       );
			s->AddMesh(mesh);
		}
		else
		{
			if (sector->m_floorHeight < otherSector->m_floorHeight)
			{
				float floorHeight = sector->m_floorHeight;
				float ceilingHeight = otherSector->m_floorHeight;

				Mesh *mesh = new Mesh();
				Vec3f *vecs = new Vec3f[4];
				vecs[0].x = (float)v0->m_x; vecs[0].y = floorHeight;   vecs[0].z = (float)v0->m_z;
				vecs[1].x = (float)v1->m_x; vecs[1].y = floorHeight;   vecs[1].z = (float)v1->m_z;
				vecs[2].x = (float)v0->m_x; vecs[2].y = ceilingHeight; vecs[2].z = (float)v0->m_z;
				vecs[3].x = (float)v1->m_x; vecs[3].y = ceilingHeight; vecs[3].z = (float)v1->m_z;
				mesh->SetVertexBuffer(vecs, 4);
				mesh->SetTexture(texture);
				Vec2f *texBuf = new Vec2f[4];
				texBuf[0].x=0; texBuf[0].y=0;
				texBuf[1].x=1; texBuf[1].y=0;
				texBuf[2].x=0; texBuf[2].y=1;
				texBuf[3].x=1; texBuf[3].y=1;
				unsigned int *indexBuf = new unsigned int[4];
				indexBuf[0]=0; indexBuf[1]=1; indexBuf[2]=3; indexBuf[3]=2;
				mesh->AddFan(indexBuf, texBuf, 4);
				s->AddMesh(mesh);
			}
			if (sector->m_ceilingHeight > otherSector->m_ceilingHeight)
			{
				float floorHeight = otherSector->m_ceilingHeight;
				float ceilingHeight = sector->m_ceilingHeight;

				Mesh *mesh = new Mesh();
				Vec3f *vecs = new Vec3f[4];
				vecs[0].x = (float)v0->m_x; vecs[0].y = floorHeight;   vecs[0].z = (float)v0->m_z;
				vecs[1].x = (float)v1->m_x; vecs[1].y = floorHeight;   vecs[1].z = (float)v1->m_z;
				vecs[2].x = (float)v0->m_x; vecs[2].y = ceilingHeight; vecs[2].z = (float)v0->m_z;
				vecs[3].x = (float)v1->m_x; vecs[3].y = ceilingHeight; vecs[3].z = (float)v1->m_z;
				mesh->SetVertexBuffer(vecs, 4);
				mesh->SetTexture(texture);
				Vec2f *texBuf = new Vec2f[4];
				texBuf[0].x=0; texBuf[0].y=0;
				texBuf[1].x=1; texBuf[1].y=0;
				texBuf[2].x=0; texBuf[2].y=1;
				texBuf[3].x=1; texBuf[3].y=1;
				unsigned int *indexBuf = new unsigned int[4];
				indexBuf[0]=0; indexBuf[1]=1; indexBuf[2]=3; indexBuf[3]=2;
				mesh->AddFan(indexBuf, texBuf, 4);
				s->AddMesh(mesh);
			}
		}
	}

	/*
	count = level->m_sectors.size();
	for (unsigned int i=0 ; i<count ; i++)
	{
		doom::Sector *sector = level->m_sectors[i];
		
		// Build the list of vertices of the sector
		std::list<doom::Vertex*> vertexes;
		for (std::list<doom::SideDef*>::iterator it=sector->m_sideDefs.begin() ; it!=sector->m_sideDefs.end() ; ++it)
		{
			vertexes.push_back(((doom::SideDef*)(*it))->m_lineDef->m_start_vtx);
			vertexes.push_back(((doom::SideDef*)(*it))->m_lineDef->m_end_vtx);
		}
		vertexes.unique(); // Remove vertices that were added twice

		// Sectors may be composed of more than one unconnected closed polygons
		// We need to tesselate each polygon one by one separately
		std::list<std::list<doom::Vertex*>> polygons;
		while (!vertexes.empty())
		{
			std::list<doom::Vertex*> polygon;
			doom::Vertex *firstVertex = vertexes.front();
			vertexes.pop_front();

			doom::Vertex *nextVertex = NULL;
			do {
				// Find which is the next vertex
			}while (nextVertex != firstVertex);
			
			polygons.push_back(polygon);
		}
	}*/

	return s;
}

Mesh* CreateMesh(doom::Vertex v0, doom::Vertex v1, int low, int high, Tex *tex)
{
	float wallLength = sqrtf((float)(v1.m_x-v0.m_x)*(v1.m_x-v0.m_x)+(float)(v1.m_z-v0.m_z)*(v1.m_z-v0.m_z));
	int wallHeight = high-low;
	Mesh *mesh = new Mesh();
	mesh->SetTexture(tex);

	float nbTexX = (float) wallLength / tex->m_mipmaps[0].m_width; // Nb of times the tex repeats on X
	float nbTexY = (float) wallHeight / tex->m_mipmaps[0].m_height;// Nb of times the tex repeats on Y
	float texStepX = (v1.m_x-v0.m_x) / nbTexX; // Length of a texture in map X
	float texStepZ = (v1.m_z-v0.m_z) / nbTexX; // Length of a texture in map Z

	// Create Vertex buffer
	int nbVertexX = 1+(int)ceilf(nbTexX);
	int nbVertexY = 1+(int)ceilf(nbTexY);
	Vec3f *vertexBuf = new Vec3f[nbVertexX*nbVertexY];

	// Preset texture coords
	Vec2f t0, t1, t2, t3, t1bis, t2bis, t3bis;
	t0.x=0.0f;                              t0.y=(float) tex->m_mipmaps[0].m_height;
	t1.x=(float) tex->m_mipmaps[0].m_width; t1.y=(float) tex->m_mipmaps[0].m_height;
	t2.x=0.0f;                              t2.y=0.0f;
	t3.x=(float) tex->m_mipmaps[0].m_width; t3.y=0.0f;
	t1bis.x=(float) tex->m_mipmaps[0].m_width*(nbTexX-floorf(nbTexX)); t1bis.y=(float) tex->m_mipmaps[0].m_height;
	t2bis.x=0.0f;                                                      t2bis.y=(float) wallHeight-wallHeight*(1-(nbTexY-floorf(nbTexY)));
	t3bis.x=(float) tex->m_mipmaps[0].m_width*(nbTexX-floorf(nbTexX)); t3bis.y=(float) wallHeight-wallHeight*(1-(nbTexY-floorf(nbTexY)));


	int v=0; // vertexBuffer index
	for (float y=0 ; y<wallHeight ; y+=tex->m_mipmaps[0].m_height)
	{
		for (float x=0 ; x<nbTexX ; x+=1.0f)
		{
			// Create texture buffer
			Vec2f *texBuf = new Vec2f[6];
			texBuf[0]=t0; texBuf[1]=t1; texBuf[2]=t3;
			texBuf[3]=t0; texBuf[4]=t3; texBuf[5]=t2;
			if (x+1.0f>nbTexX && y+tex->m_mipmaps[0].m_height<wallHeight)
			{
				texBuf[1]=t1bis; texBuf[2]=t3bis;
				texBuf[4]=t3bis; texBuf[5]=t2bis;
			}
			else if (x+1.0f>nbTexX)
			{
				texBuf[1]=t1bis; texBuf[2]=t3bis;
				texBuf[4]=t3bis;
			}
			else if (y+tex->m_mipmaps[0].m_height>wallHeight)
			{
				texBuf[2]=t3bis;
				texBuf[4]=t3bis; texBuf[5]=t2bis;
			}

			// Create Index buff
			unsigned int *indexBuf = new unsigned int[4];
			indexBuf[0]=v; indexBuf[1]=v+1; indexBuf[2]=v+1+nbVertexX; indexBuf[3]=v+nbVertexX;
			mesh->AddFan(indexBuf, texBuf, 4);

			// Set vertex
			vertexBuf[v].x = v0.m_x + x*texStepX;
			vertexBuf[v].y = y;
			vertexBuf[v].z = v0.m_z + x*texStepZ;
			v++;
		}
		// last vertex on row
		vertexBuf[v].x = (float) v1.m_x;
		vertexBuf[v].y = y;
		vertexBuf[v].z = (float) v1.m_z;
		v++; // next row
	}
	// last row
	for (float x=0 ; x<nbTexX ; x+=1.0f)
	{
		vertexBuf[v].x = v0.m_x + x*texStepX;
		vertexBuf[v].y = (float) high;
		vertexBuf[v].z = v0.m_z + x*texStepZ;
		v++;
	}
	// last vertex on row
	vertexBuf[v].x = (float) v1.m_x;
	vertexBuf[v].y = (float) high;
	vertexBuf[v].z = (float) v1.m_z;


	/*
	int i=1;
	while (nbReps > 1.0f)
	{
		Vec2f t0, t1, t2, t3;
		t0.x=0;                         t0.y=tex->m_mipmaps[0].m_height;
		t1.x=0;                         t1.y=0;
		t2.x=tex->m_mipmaps[0].m_width; t2.y=tex->m_mipmaps[0].m_height;
		t3.x=tex->m_mipmaps[0].m_width; t3.y=0;
		Vec2f *texBuf = new Vec2f[6];
		texBuf[0]=t0; texBuf[1]=t2; texBuf[2]=t3;
		texBuf[3]=t0; texBuf[4]=t3; texBuf[5]=t1; 
		unsigned int *indexBuf = new unsigned int[4];
		indexBuf[0]=0; indexBuf[1]=2; indexBuf[2]=3; indexBuf[3]=1;
		mesh->AddFan(indexBuf, texBuf, 4);

		nbReps -= 1.0f;
		i++;
	}

	if (nbReps > 0)
	{
		//last chunk
		Vec2f t0, t1, t2, t3;
		t0.x=0;          t0.y=tex->m_mipmaps[0].m_height;
		t1.x=0;          t1.y=0;
		t2.x=wallLength; t2.y=tex->m_mipmaps[0].m_height;
		t3.x=wallLength; t3.y=0;
		Vec2f *texBuf = new Vec2f[6];
		texBuf[0]=t0; texBuf[1]=t2; texBuf[2]=t3;
		texBuf[3]=t0; texBuf[4]=t3; texBuf[5]=t1; 
		unsigned int *indexBuf = new unsigned int[4];
		indexBuf[0]=0; indexBuf[1]=2; indexBuf[2]=3; indexBuf[3]=1;
		mesh->AddFan(indexBuf, texBuf, 4);
	}
	*/

	mesh->SetVertexBuffer(vertexBuf, nbVertexX*nbVertexY);
	return mesh;
}

void VideoWorks()
{
	LARGE_INTEGER microseconds1;

	// Lock surface if needed
	if (SDL_MUSTLOCK(g_screen)) 
		if (SDL_LockSurface(g_screen) < 0) 
			return;

	if ((g_frameCount % 100) == 0) {
		QueryPerformanceCounter(&microseconds1);
	}

	g_camera->RotateY(g_camerayrotation);
	g_camera->RotateX(g_cameraxrotation);
	renderer1->Render();
	g_camera->RotateX(-g_cameraxrotation);
	g_camera->RotateY(-g_camerayrotation);
	
	if ((g_frameCount % 100) == 0) {
		LARGE_INTEGER microseconds2;
		QueryPerformanceCounter(&microseconds2);
		g_renderingTime.QuadPart = microseconds2.QuadPart - microseconds1.QuadPart;
		
		LARGE_INTEGER freq;			
		QueryPerformanceFrequency(&freq);
		double seconds = g_renderingTime.QuadPart / (1.0f*freq.QuadPart);
		char tmp[256];
		sprintf_s(tmp, "Time per frame is %e s\n", seconds);
		TCHAR tmp2[512];
		wsprintf(tmp2, L"%S", tmp);
		OutputDebugString(tmp2);
	}
	g_frameCount++;

	// Unlock if needed
	if (SDL_MUSTLOCK(g_screen)) 
		SDL_UnlockSurface(g_screen);

	// Tell SDL to update the whole screen
	SDL_UpdateRect(g_screen, 0, 0, g_scr_w, g_scr_h); 
}

int HandleGameplayInput(SDL_Event event)
{
	static int g_keystick = SDL_GetTicks();
	
	while ( (SDL_GetTicks())-g_keystick > KEYBOARD_RATE_MS ) // Limit the keyrate
	{
		if (g_mouse_x != 0 || g_mouse_y != 0)
		{
			//g_camera->RotateX(-g_cameraxrotation);
			//g_camera->RotateY(g_mouse_x / 2000.0f);
			g_cameraxrotation += g_mouse_y / 1000.0f;
			g_camerayrotation += g_mouse_x / 1000.0f;
			//g_camera->RotateX(g_cameraxrotation);
			g_mouse_x = 0;
			g_mouse_y = 0;
		}
		/*
		if (g_keys['d'])
			g_camera->RotateY(KEYBOARD_RATE_MS * 3.14159f / 2000.0f);
		if (g_keys['a'])
			g_camera->RotateY(KEYBOARD_RATE_MS * 3.14159f / -2000.0f);
		if (g_keys['w'])
			g_camera->RotateX(KEYBOARD_RATE_MS * 3.14159f / 2000.0f);
		if (g_keys['s'])
			g_camera->RotateX(KEYBOARD_RATE_MS * 3.14159f / -2000.0f);
		if (g_keys['e'])
			g_camera->RotateZ(KEYBOARD_RATE_MS * 3.14159f / 2000.0f);
		if (g_keys['q'])
			g_camera->RotateZ(KEYBOARD_RATE_MS * 3.14159f / -2000.0f);
		*/

		if (g_keys['w'] || g_keys['s'] || g_keys['d'] || g_keys['a'] || g_keys['r'] || g_keys['f'])
		{
			g_camera->RotateY(g_camerayrotation);

			if (g_keys['w'])
				g_camera->Translate(0.0f, 0.0f, KEYBOARD_RATE_MS * 0.4f);
			if (g_keys['s'])
				g_camera->Translate(0.0f, 0.0f, KEYBOARD_RATE_MS * -0.4f);
			if (g_keys['d'])
				g_camera->Translate(KEYBOARD_RATE_MS * 0.4f, 0.0f, 0.0f);
			if (g_keys['a'])
				g_camera->Translate(KEYBOARD_RATE_MS * -0.4f, 0.0f, 0.0f);
			if (g_keys['r'])
				g_camera->Translate(0.0f, KEYBOARD_RATE_MS * 0.4f, 0.0f);
			if (g_keys['f'])
				g_camera->Translate(0.0f, KEYBOARD_RATE_MS * -0.4f, 0.0f);

			g_camera->RotateY(-g_camerayrotation);

			printf("Camera coords = %f, %f\n", g_camera->m_viewmatrix.m_data[3], g_camera->m_viewmatrix.m_data[11]);
		}

		if (g_keys['t'])
			g_zoom *= 1.01f;
		if (g_keys['g'])
			g_zoom /= 1.01f;

		g_keystick += KEYBOARD_RATE_MS;
	}
	if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == SDLK_h)
		{
			if (g_gridZoom < 0x40000000)
				g_gridZoom *= 2;
		}
		else if (event.key.keysym.sym == SDLK_y)
		{
			if (g_gridZoom > 1)
				g_gridZoom /= 2;
		}
	}
	return 0;
}