
#include <windows.h>
#include <math.h>
#include <stdlib.h>

#include <SDL/SDL.h>
#include <swegl/swegl.hpp>

#include "gameplay.h"
#include "minimap.h"
#include "input.h"

#include "doom/Thing.h"
#include "doom/SideDef.h"
#include "doom/LineDef.h"
#include "doom/Vertex.h"
#include "doom/Sector.h"
#include "doom/Texture.h"
#include "doom/lumps/PatchLump.h"


// From display.cpp
extern unsigned int g_scr_w;
extern unsigned int g_scr_h;

// From main.cpp

LARGE_INTEGER g_renderingTime;
unsigned int g_frameCount = 0;

// From minimap.cpp
extern float g_zoom;
extern int g_gridZoom;

swegl::Scene* BuildScene(skulls::Level & level);
swegl::Mesh * CreateMesh(skulls::Vertex v0, skulls::Vertex v1, int low, int high, std::shared_ptr<skulls::Texture> tex, int xoff, int yoff);
void VideoWorks(SDL_Surface * screen);
int HandleGameplayInput();

static swegl::Scene *scene;
swegl::Camera *g_camera;
static swegl::ViewPort *viewport1;
//static swegl::Renderer *renderer1 = NULL;
static swegl::R008NoTexelArtefact *renderer1 = NULL;

	
static float g_cameraxrotation;
static float g_camerayrotation;

void PlayLevel(skulls::Level & level, SDL_Surface * screen)
{	
	scene = BuildScene(level);

	g_camera = new swegl::Camera(g_scr_w/(1.0f*g_scr_h));
	viewport1 = new swegl::ViewPort(0, 0, g_scr_w, g_scr_h, screen);
	//renderer1 = new swegl::Renderer(scene, g_camera, viewport1);
	renderer1 = new swegl::R008NoTexelArtefact(scene, g_camera, viewport1);

	g_camera->Translate(level.m_things[0].m_x, 0.0f, level.m_things[0].m_z);

	//ShowMinimap(screen, level);
	
	while (1)
	{
		VideoWorks(screen);
		SDL_Event event = RefreshKeybState();
		if (event.type == SDL_QUIT)
			exit(0);
		else if (event.type==SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_TAB)
				ShowMinimap(screen, level);
			else if (event.key.keysym.sym == SDLK_ESCAPE)
				return;
		}
		HandleGameplayInput(event);
	}
	
	return;
}

swegl::Scene* BuildScene(skulls::Level & level)
{
	swegl::Scene *s = new swegl::Scene();
	unsigned int count;

	/*
	count = (unsigned int) level->m_things.size();
	for (unsigned int i=0 ; i<count ; i++)
	{
		skulls::Thing * thingLump = level->m_things[i];

		swegl::Texture *texture = new swegl::Texture(thingLump->m_sprites[0]->m_bitmap,
		                                             thingLump->m_sprites[0]->m_w,
		                                             thingLump->m_sprites[0]->m_h);

		swegl::Cube *c = new swegl::Cube(texture->m_mipmaps[0].m_height*1.0f, texture);
		c->m_worldmatrix.Translate(thingLump->m_x, 0.0f, thingLump->m_z);
		s->AddMesh(c);
	}
	/**/

	swegl::Texture *texture = new swegl::Texture(0x00FFFFFF);
	count = (unsigned int) level.m_sideDefs.size();
	for (unsigned int i=0 ; i<count ; i++)
	{
		std::shared_ptr<skulls::SideDef> sideDef = level.m_sideDefs[i];
		std::shared_ptr<skulls::Vertex> v0;
		std::shared_ptr<skulls::Vertex> v1;
		std::shared_ptr<skulls::Sector> otherSector;
		std::shared_ptr<skulls::Sector> sector = sideDef->m_sector;
		otherSector = nullptr;
		if (sideDef == sideDef->m_lineDef->m_rightSideDef)
		{
			v0 = sideDef->m_lineDef->m_start_vtx;
			v1 = sideDef->m_lineDef->m_end_vtx;
			if (sideDef->m_lineDef->m_leftSideDef)
				otherSector = sideDef->m_lineDef->m_leftSideDef->m_sector;
		}
		else
		{
			v1 = sideDef->m_lineDef->m_start_vtx;
			v0 = sideDef->m_lineDef->m_end_vtx;
			if (sideDef->m_lineDef->m_rightSideDef)
				otherSector = sideDef->m_lineDef->m_rightSideDef->m_sector;
		}
		if (otherSector == NULL && sideDef->m_middleTexture)
		{
			swegl::Mesh * mesh = CreateMesh(*v0, *v1,
			                                sideDef->m_sector->m_floorHeight,
			                                sideDef->m_sector->m_ceilingHeight,
			                                sideDef->m_middleTexture,
			                                sideDef->m_xoffset,
			                                sideDef->m_yoffset
			                               );
			if (mesh != nullptr)
				s->AddMesh(mesh);
		}
		else
		{
			if (sector->m_floorHeight < otherSector->m_floorHeight && sideDef->m_lowerTexture)
			{
				swegl::Mesh * mesh = CreateMesh(*v0, *v1,
				                                sector->m_floorHeight,
				                                otherSector->m_floorHeight,
				                                sideDef->m_lowerTexture,
				                                sideDef->m_xoffset,
				                                sideDef->m_yoffset
				                               );
				if (mesh != nullptr)
					s->AddMesh(mesh);
			}
			if (sector->m_ceilingHeight > otherSector->m_ceilingHeight && sideDef->m_upperTexture != nullptr)
			{
				swegl::Mesh *mesh = CreateMesh(*v0, *v1,
				                               otherSector->m_ceilingHeight,
				                               sector->m_ceilingHeight,
				                               sideDef->m_upperTexture,
				                               sideDef->m_xoffset,
				                               sideDef->m_yoffset
				                              );
				if (mesh != nullptr)
					s->AddMesh(mesh);
			}
		}
	}

	/*
	count = level->m_sectors.size();
	for (unsigned int i=0 ; i<count ; i++)
	{
		skulls::Sector *sector = level->m_sectors[i];
		
		// Build the list of vertices of the sector
		std::list<skulls::Vertex*> vertexes;
		for (std::list<skulls::SideDef*>::iterator it=sector->m_sideDefs.begin() ; it!=sector->m_sideDefs.end() ; ++it)
		{
			vertexes.push_back(((skulls::SideDef*)(*it))->m_lineDef->m_start_vtx);
			vertexes.push_back(((skulls::SideDef*)(*it))->m_lineDef->m_end_vtx);
		}
		vertexes.unique(); // Remove vertices that were added twice

		// Sectors may be composed of more than one unconnected closed polygons
		// We need to tesselate each polygon one by one separately
		std::list<std::list<skulls::Vertex*>> polygons;
		while (!vertexes.empty())
		{
			std::list<skulls::Vertex*> polygon;
			skulls::Vertex *firstVertex = vertexes.front();
			vertexes.pop_front();

			skulls::Vertex *nextVertex = NULL;
			do {
				// Find which is the next vertex
			}while (nextVertex != firstVertex);
			
			polygons.push_back(polygon);
		}
	}*/

	return s;
}


swegl::Mesh * CreateMesh(skulls::Vertex v0, skulls::Vertex v1, int low, int high, std::shared_ptr<skulls::Texture> tex, int xoff, int yoff)
{
	//if (xoff != 0 || yoff != 0)
	//	return nullptr;
	
	xoff = xoff % tex->GetWidth();
	yoff = yoff % tex->GetHeight();
	
	//if (xoff != 0 /*|| yoff == 0*/)
	//	return nullptr;

	float wallLength = sqrtf((float)(v1.m_x-v0.m_x)*(v1.m_x-v0.m_x)+(float)(v1.m_z-v0.m_z)*(v1.m_z-v0.m_z));
	int wallHeight = high-low;
	
	int nbTexX = 0;
	if (xoff > 0)
		nbTexX++;
	for (float wx=wallLength-xoff ; wx>0 ; nbTexX++)
		wx -= min(wx, tex->GetWidth());
	int nbTexY = 0;
	if (yoff > 0)
		nbTexY++;
	for (int wy=wallHeight-yoff ; wy>0 ; nbTexY++)
		wy -= min(wy, tex->GetHeight());
	
	//if (nbTexX == 1 || nbTexY == 1)
	//	return nullptr;

	swegl::Mesh * mesh = new swegl::Mesh();
	mesh->SetTexture(new swegl::Texture(tex->GetBitmap(), tex->GetWidth(), tex->GetHeight()));
	std::vector<swegl::Vec3f> vertexBuf;
	vertexBuf.resize((nbTexX+1)*(nbTexY+1), swegl::Vec3f());
		
	// Preset texture coords
	swegl::Vec2f t0, t1, t2, t3;
	t0.x = 0;                                             t0.y = (float) mesh->m_texture->m_mipmaps[0].m_height;
	t1.x = (float) mesh->m_texture->m_mipmaps[0].m_width; t1.y = (float) mesh->m_texture->m_mipmaps[0].m_height;
	t2.x = 0;                                             t2.y = 0;
	t3.x = (float) mesh->m_texture->m_mipmaps[0].m_width; t3.y = 0;

	{
		float vertex_coord_y = (float) high;
		for (int i=0 ; i<=nbTexX ; i++)
		{
			vertexBuf[i].y = vertex_coord_y;
			vertexBuf[(nbTexX+1)*nbTexY + i].y = (float) low;
		}
		vertex_coord_y -= (yoff!=0)?yoff:tex->GetHeight();
		for (int j=1 ; j<nbTexY ; j++)
		{
			for (int i=0 ; i<nbTexX+1 ; i++)
				vertexBuf[j*(nbTexX+1) + i].y = max(low, vertex_coord_y);
			vertex_coord_y -= tex->GetHeight();
		}
	}
	{
		float vertex_coord_x = (float) v0.m_x;
		float vertex_coord_z = (float) v0.m_z;

		float wallLengthRemainder = wallLength;
		
		for (int j=0 ; j<=nbTexY ; j++)
		{
			vertexBuf[j*(nbTexX+1)].x        = (float) v0.m_x;
			vertexBuf[j*(nbTexX+1)].z        = (float) v0.m_z;
			vertexBuf[j*(nbTexX+1)+nbTexX].x = (float) v1.m_x;
			vertexBuf[j*(nbTexX+1)+nbTexX].z = (float) v1.m_z;
		}
		vertex_coord_x += (v1.m_x - v0.m_x) * min(wallLengthRemainder,(xoff!=0 ? xoff : tex->GetWidth())) / wallLength;
		vertex_coord_z += (v1.m_z - v0.m_z) * min(wallLengthRemainder,(xoff!=0 ? xoff : tex->GetWidth())) / wallLength;
		wallLengthRemainder -= wallLength * min(wallLengthRemainder,(xoff!=0 ? xoff : tex->GetWidth())) / wallLength;

		for (int i=1 ; i<nbTexX ; i++)
		{
			for (int j=0 ; j<=nbTexY ; j++)
			{
				vertexBuf[j*(nbTexX+1) + i].x = vertex_coord_x;
				vertexBuf[j*(nbTexX+1) + i].z = vertex_coord_z;
			}
			vertex_coord_x += (v1.m_x - v0.m_x) * min(wallLengthRemainder,tex->GetWidth()) / wallLength;
			vertex_coord_z += (v1.m_z - v0.m_z) * min(wallLengthRemainder,tex->GetWidth()) / wallLength;
			wallLengthRemainder -= wallLength * min(wallLengthRemainder,(xoff!=0 ? xoff : tex->GetWidth())) / wallLength;
		}
	}

	for (int j=0 ; j<nbTexY ; j++)
	{
		for (int i=0 ; i<nbTexX ; i++)
		{
			// Create texture buffer
			std::vector<swegl::Vec2f> texBuf;
			texBuf.reserve(6);
			texBuf.emplace_back(t0);
			texBuf.emplace_back(t1);
			texBuf.emplace_back(t3);
			texBuf.emplace_back(t0);
			texBuf.emplace_back(t3);
			texBuf.emplace_back(t2);

			// Create Index buff
			std::vector<unsigned int> indexBuf;
			indexBuf.reserve(4);
			indexBuf.emplace_back((j+1)*(nbTexX+1) + i    );
			indexBuf.emplace_back((j+1)*(nbTexX+1) + i + 1);
			indexBuf.emplace_back((j  )*(nbTexX+1) + i + 1);
			indexBuf.emplace_back((j  )*(nbTexX+1) + i    );
			mesh->AddFan(std::move<>(indexBuf), std::move<>(texBuf));
		}
	}

	mesh->SetVertexBuffer(std::move<>(vertexBuf));
	return mesh;
}


#if 0
swegl::Mesh* CreateMesh(skulls::Vertex v0, skulls::Vertex v1, int low, int high, std::shared_ptr<skulls::Texture> tex)
{
	float wallLength = sqrtf((float)(v1.m_x-v0.m_x)*(v1.m_x-v0.m_x)+(float)(v1.m_z-v0.m_z)*(v1.m_z-v0.m_z));
	int wallHeight = high-low;
	swegl::Mesh *mesh = new swegl::Mesh();
	mesh->SetTexture(new swegl::Texture(tex->GetBitmap(), tex->GetWidth(), tex->GetHeight()));

	float nbTexX = (float) wallLength / mesh->m_texture->m_mipmaps[0].m_width; // Nb of times the tex repeats on X
	float nbTexY = (float) wallHeight / mesh->m_texture->m_mipmaps[0].m_height;// Nb of times the tex repeats on Y
	float texStepX = (v1.m_x-v0.m_x) / nbTexX; // Length of a texture in map X
	float texStepZ = (v1.m_z-v0.m_z) / nbTexX; // Length of a texture in map Z

	// Create Vertex buffer
	int nbVertexX = 1+(int)ceilf(nbTexX);
	int nbVertexY = 1+(int)ceilf(nbTexY);
	swegl::Vec3f *vertexBuf = new swegl::Vec3f[nbVertexX*nbVertexY];

	// Preset texture coords
	swegl::Vec2f t0, t1, t2, t3, t1bis, t2bis, t3bis;
	t0.x=0.0f;                              t0.y=(float) mesh->m_texture->m_mipmaps[0].m_height;
	t1.x=(float) mesh->m_texture->m_mipmaps[0].m_width; t1.y=(float) mesh->m_texture->m_mipmaps[0].m_height;
	t2.x=0.0f;                              t2.y=0.0f;
	t3.x=(float) mesh->m_texture->m_mipmaps[0].m_width; t3.y=0.0f;
	t1bis.x=(float) mesh->m_texture->m_mipmaps[0].m_width*(nbTexX-floorf(nbTexX)); t1bis.y=(float) mesh->m_texture->m_mipmaps[0].m_height;
	t2bis.x=0.0f;                                                      t2bis.y=(float) wallHeight-wallHeight*(1-(nbTexY-floorf(nbTexY)));
	t3bis.x=(float) mesh->m_texture->m_mipmaps[0].m_width*(nbTexX-floorf(nbTexX)); t3bis.y=(float) wallHeight-wallHeight*(1-(nbTexY-floorf(nbTexY)));


	int v=0; // vertexBuffer index
	for (float y=0 ; y<wallHeight ; y+=mesh->m_texture->m_mipmaps[0].m_height)
	{
		for (float x=0 ; x<nbTexX ; x+=1.0f)
		{
			// Create texture buffer
			swegl::Vec2f *texBuf = new swegl::Vec2f[6];
			texBuf[0]=t0; texBuf[1]=t1; texBuf[2]=t3;
			texBuf[3]=t0; texBuf[4]=t3; texBuf[5]=t2;
			if (x+1.0f>nbTexX && y+mesh->m_texture->m_mipmaps[0].m_height<wallHeight)
			{
				texBuf[1]=t1bis; texBuf[2]=t3bis;
				texBuf[4]=t3bis; texBuf[5]=t2bis;
			}
			else if (x+1.0f>nbTexX)
			{
				texBuf[1]=t1bis; texBuf[2]=t3bis;
				texBuf[4]=t3bis;
			}
			else if (y+mesh->m_texture->m_mipmaps[0].m_height>wallHeight)
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


swegl::Mesh * CreateMesh(skulls::Vertex v0, skulls::Vertex v1, int low, int high, std::shared_ptr<skulls::Texture> tex)
{
	swegl::Mesh * mesh = new swegl::Mesh();
	mesh->SetTexture(new swegl::Texture(tex->GetBitmap(), tex->GetWidth(), tex->GetHeight()));

	swegl::Vec3f *vertexBuf = new swegl::Vec3f[4];

	// Preset texture coords
	swegl::Vec2f t0, t1, t2, t3;
	t0.x = 0.0f;                                          t0.y = (float) mesh->m_texture->m_mipmaps[0].m_height;
	t1.x = (float) mesh->m_texture->m_mipmaps[0].m_width; t1.y = (float) mesh->m_texture->m_mipmaps[0].m_height;
	t2.x = 0.0f;                                          t2.y = 0.0f;
	t3.x = (float) mesh->m_texture->m_mipmaps[0].m_width; t3.y = 0.0f;

	// Create texture buffer
	swegl::Vec2f *texBuf = new swegl::Vec2f[6];
	texBuf[0]=t0; texBuf[1]=t1; texBuf[2]=t3;
	texBuf[3]=t0; texBuf[4]=t3; texBuf[5]=t2;
		
	// Create Index buff
	unsigned int *indexBuf = new unsigned int[4];
	indexBuf[0]=0; indexBuf[1]=1; indexBuf[2]=3; indexBuf[3]=2;
	mesh->AddFan(indexBuf, texBuf, 4);

	// Set vertex
	vertexBuf[0].x = (float) v0.m_x;
	vertexBuf[0].y = (float) low;
	vertexBuf[0].z = (float) v0.m_z;
	vertexBuf[1].x = (float) v1.m_x;
	vertexBuf[1].y = (float) low;
	vertexBuf[1].z = (float) v1.m_z;
	vertexBuf[2].x = (float) v0.m_x;
	vertexBuf[2].y = (float) high;
	vertexBuf[2].z = (float) v0.m_z;
	vertexBuf[3].x = (float) v1.m_x;
	vertexBuf[3].y = (float) high;
	vertexBuf[3].z = (float) v1.m_z;

	mesh->SetVertexBuffer(vertexBuf, 4);
	return mesh;
}

#endif

void VideoWorks(SDL_Surface * screen)
{
	LARGE_INTEGER microseconds1;

	// Lock surface if needed
	if (SDL_MUSTLOCK(screen)) 
		if (SDL_LockSurface(screen) < 0) 
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
		wsprintf(tmp2, "%S", tmp);
		OutputDebugString(tmp2);
	}
	g_frameCount++;

	// Unlock if needed
	if (SDL_MUSTLOCK(screen)) 
		SDL_UnlockSurface(screen);

	// Tell SDL to update the whole screen
	SDL_UpdateRect(screen, 0, 0, g_scr_w, g_scr_h); 
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