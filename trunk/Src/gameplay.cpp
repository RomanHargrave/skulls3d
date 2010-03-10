
#include "gameplay.h"


#include <stdlib.h>
#if defined(_MSC_VER)
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif

#include <windows.h>

#include "ViewPort.h"
#include "Scene.h"
#include "Matrix4x4.h"
#include "Renderer.h"
#include "Filler.h"
//#include "font.h"
#include "Texture.h"
#include "Mesh.h"
#include "Cube.h"
#include "Tore.h"
#include "RectangleTriangle.h"

// From display.cpp
extern unsigned int g_scr_w;
extern unsigned int g_scr_h;

// From main.cpp
extern SDL_Surface *g_screen;

Scene* BuildScene(doom::LevelLump * level);
void VideoWorks();
int KeyboardWorks();

Scene *scene;
Camera *camera;
ViewPort *viewport1;
ViewPort *viewport2;
Renderer *renderer1 = NULL;
Renderer *renderer2 = NULL;
//Font font("ascii.bmp");
char keys[256];
int zoom = 0;
	
#if defined(_DEBUG) || defined(DEBUG)
	unsigned int g_trianglesdrawn;
	unsigned int g_pixelsdrawn;
#endif 

#if defined(_DEBUG) || defined(DEBUG)
void AssertFailed(char * cond, char * filename, int line)
{
//	char tmp[1024];
//	sprintf(tmp, "[%s] is FALSE at l.%d of %s\n", cond, line, filename);
//	OutputDebugString(tmp);
}
#endif

void PlayLevel(doom::LevelLump * level)
{
	if (level == NULL)
		return;
	if (level->Load() != 0)
		return;
	
	memset(keys, 0, 256);

	scene = BuildScene(level);

	//*
	camera = new Camera(1.0f * g_scr_w/g_scr_h);
	viewport1 = new ViewPort(0,0, g_scr_w,g_scr_h,g_screen);
	renderer1 = new Renderer(scene, camera, viewport1);
	/**/
	/*
	camera = new Camera((g_scr_w/2.0f)/g_scr_h);
	viewport1 = new ViewPort(0,0,           g_scr_w/2,g_scr_h,screen);
	viewport2 = new ViewPort(g_scr_w/2,0, g_scr_w/2,g_scr_h,screen);
	renderer1 = new R007Bilinear(scene, camera, viewport1);
	renderer2 = new R008NoTexelArtefact(scene, camera, viewport2);
	/**/
	/*
	camera = new Camera((g_scr_w)/(g_scr_h/2.0f));
	viewport1 = new ViewPort(0,0,            g_scr_w,g_scr_h/2,screen);
	viewport2 = new ViewPort(0,g_scr_h/2, g_scr_w,g_scr_h/2,screen);
	renderer1 = new R008NoTexelArtefact(scene, camera, viewport1);
	renderer2 = new R009Antialiasing(scene, camera, viewport2);
	/**/
	while (1)
	{
		VideoWorks();
		if (int a=KeyboardWorks() < 0)
			return;
	}
	return;
}

Scene* BuildScene(doom::LevelLump * level)
{
	Texture *t = new Texture("tex.bmp");
	Texture *bumpmap = new Texture("bumpmap.bmp");
	Scene *s = new Scene();
	//*
	Tore *tore = new Tore(20, t);
	tore->m_worldmatrix.Translate(0.0f, 0.0f, 8.0f);
	//tore->SetBumpMap(bumpmap);
	s->AddMesh(tore);
	/**/

	//*
	Cube *c = new Cube(1.0f, t);
	c->m_worldmatrix.RotateX(0.5f);
	c->m_worldmatrix.RotateZ(1.5f);
	c->m_worldmatrix.Translate(0.0f, 0.5f, 6.0f);
	//c->SetBumpMap(bumpmap);
	s->AddMesh(c);
	/**/

	/*
	Texture *dummy_texture = new Texture(0x00FF00FF);
	RectangleTriangle *tri = new RectangleTriangle(0.5f, 0.5f, dummy_texture);
	tri->m_worldmatrix.Translate(-0.2f, -0.2f, 0.8f);
	s->AddMesh(tri);
	/**/
	return s;
}

void VideoWorks()
{
	static int fpsticks = SDL_GetTicks();
	//char fps[32];

	// Lock surface if needed
	if (SDL_MUSTLOCK(g_screen)) 
		if (SDL_LockSurface(g_screen) < 0) 
			return;

	renderer1->Render();
	if (renderer2 != NULL)
		renderer2->Render();

	// Print fps
	/*
	sprintf_s(fps, "%.2f fps", 1000.0f/(SDL_GetTicks()-fpsticks));
	font.Print(fps, 10, 10, g_screen);
	fpsticks = SDL_GetTicks();
	#if defined(_DEBUG) || defined(DEBUG)
		sprintf_s(fps, "%d tris, %d pixels", g_trianglesdrawn, g_pixelsdrawn);
		font.Print(fps, 10, 26, screen);
		g_trianglesdrawn = g_pixelsdrawn = 0;
	#endif
	*/

	// Unlock if needed
	if (SDL_MUSTLOCK(g_screen)) 
		SDL_UnlockSurface(g_screen);

	// Tell SDL to update the whole screen
	SDL_UpdateRect(g_screen, 0, 0, g_scr_w, g_scr_h); 
}

int KeyboardWorks()
{
	static int keystick = SDL_GetTicks();
	static float cameraxrotation;
	SDL_Event event;
	
	// Poll for events, and handle the ones we care about.
	while (SDL_PollEvent(&event)) 
	{
		switch (event.type) 
		{
			case SDL_MOUSEMOTION:
				camera->RotateX(-cameraxrotation);
				camera->RotateY(event.motion.xrel / 2000.0f);
				cameraxrotation += event.motion.yrel / 2000.0f;
				camera->RotateX(cameraxrotation);
				break;

			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
					return -10;
				else if (event.key.keysym.sym == SDLK_d)
					keys['d'] = 1;
				else if (event.key.keysym.sym == SDLK_a)
					keys['a'] = 1;
				else if (event.key.keysym.sym == SDLK_w)
					keys['w'] = 1;
				else if (event.key.keysym.sym == SDLK_s)
					keys['s'] = 1;
				else if (event.key.keysym.sym == SDLK_e)
					keys['e'] = 1;
				else if (event.key.keysym.sym == SDLK_q)		
					keys['q'] = 1;
				else if (event.key.keysym.sym == SDLK_i)
					keys['i'] = 1;
				else if (event.key.keysym.sym == SDLK_k)
					keys['k'] = 1;
				else if (event.key.keysym.sym == SDLK_l)
					keys['l'] = 1;
				else if (event.key.keysym.sym == SDLK_j)
					keys['j'] = 1;
				else if (event.key.keysym.sym == SDLK_o)
					keys['o'] = 1;
				else if (event.key.keysym.sym == SDLK_u)
					keys['u'] = 1;
				else if (event.key.keysym.sym == SDLK_PLUS) {
					if (zoom < 3) zoom ++;
				}
				else if (event.key.keysym.sym == SDLK_MINUS) {
					if (zoom > 0) zoom --;
				}
				break;
				
			case SDL_KEYUP:
				// If escape is pressed, return (and thus, quit)

				/*
				else if (event.key.keysym.sym == SDLK_h)
				scene->m_worldmatrix->RotateY(3.14159f / 180.0f);
				else if (event.key.keysym.sym == SDLK_f)
				scene->m_worldmatrix->RotateY(3.14159f / -180.0f);
				else if (event.key.keysym.sym == SDLK_t)
				scene->m_worldmatrix->RotateX(3.14159f / 180.0f);
				else if (event.key.keysym.sym == SDLK_g)
				scene->m_worldmatrix->RotateX(3.14159f / -180.0f);
				else if (event.key.keysym.sym == SDLK_y)
				scene->m_worldmatrix->RotateZ(3.14159f / 180.0f);
				else if (event.key.keysym.sym == SDLK_r)
				scene->m_worldmatrix->RotateZ(3.14159f / -180.0f);
				*/

				if (event.key.keysym.sym == SDLK_ESCAPE)
					return -2;
				else if (event.key.keysym.sym == SDLK_d)
					keys['d'] = 0;
				else if (event.key.keysym.sym == SDLK_a)
					keys['a'] = 0;
				else if (event.key.keysym.sym == SDLK_w)
					keys['w'] = 0;
				else if (event.key.keysym.sym == SDLK_s)
					keys['s'] = 0;
				else if (event.key.keysym.sym == SDLK_e)
					keys['e'] = 0;
				else if (event.key.keysym.sym == SDLK_q)		
					keys['q'] = 0;
				else if (event.key.keysym.sym == SDLK_i)
					keys['i'] = 0;
				else if (event.key.keysym.sym == SDLK_k)
					keys['k'] = 0;
				else if (event.key.keysym.sym == SDLK_l)
					keys['l'] = 0;
				else if (event.key.keysym.sym == SDLK_j)
					keys['j'] = 0;
				else if (event.key.keysym.sym == SDLK_o)
					keys['o'] = 0;
				else if (event.key.keysym.sym == SDLK_u)
					keys['u'] = 0;
					break;
			case SDL_QUIT:
				return -3;
		}
	}

	float multiplier = (SDL_GetTicks()-keystick) / 10.0f;
	//if ( (SDL_GetTicks())-keystick > 100 )
	{
		if (keys['d'])
			camera->RotateY(multiplier * 3.14159f / 2000.0f);
		if (keys['a'])
			camera->RotateY(multiplier * 3.14159f / -2000.0f);
		if (keys['w'])
			camera->RotateX(multiplier * 3.14159f / 2000.0f);
		if (keys['s'])
			camera->RotateX(multiplier * 3.14159f / -2000.0f);
		if (keys['e'])
			camera->RotateZ(multiplier * 3.14159f / 2000.0f);
		if (keys['q'])
			camera->RotateZ(multiplier * 3.14159f / -2000.0f);

		if (keys['i'] || keys['k'] || keys['j'] || keys['l'] || keys['o'] || keys['u'])
				camera->RotateX(-cameraxrotation);

		if (keys['i'])
			camera->Translate(0.0f, 0.0f, multiplier * 0.004f);
		if (keys['k'])
			camera->Translate(0.0f, 0.0f, multiplier * -0.004f);
		if (keys['l'])
			camera->Translate(multiplier * 0.004f, 0.0f, 0.0f);
		if (keys['j'])
			camera->Translate(multiplier * -0.004f, 0.0f, 0.0f);
		if (keys['o'])
			camera->Translate(0.0f, multiplier * 0.004f, 0.0f);
		if (keys['u'])
			camera->Translate(0.0f, multiplier * -0.004f, 0.0f);

		if (keys['i'] || keys['k'] || keys['j'] || keys['l'] || keys['o'] || keys['u'])
				camera->RotateX(cameraxrotation);

		keystick = SDL_GetTicks();
	}

	return 0;
}