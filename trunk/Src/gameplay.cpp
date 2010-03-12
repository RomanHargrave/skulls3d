
#include <stdlib.h>
#if defined(_MSC_VER)
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif

#include <windows.h>

#include "gameplay.h"
#include "minimap.h"
#include "input.h"

#include "Matrix4x4.h"
#include "ViewPort.h"
#include "Scene.h"
#include "Renderer.h"
#include "Texture.h"

#include "doom/Thing.h"
#include "doom/lumps/PatchLump.h"

#include "Cube.h"

// From display.cpp
extern unsigned int g_scr_w;
extern unsigned int g_scr_h;

// From main.cpp
extern SDL_Surface *g_screen;

Scene* BuildScene(doom::LevelLump * level);
void VideoWorks();
int HandleGameplayInput();

static Scene *scene;
static Camera *camera;
static ViewPort *viewport1;
static Renderer *renderer1 = NULL;
	
static float cameraxrotation;


void PlayLevel(doom::LevelLump * level)
{
	if (level == NULL)
		return;
	if (level->Load() != 0)
		return;
	
	scene = BuildScene(level);

	camera = new Camera(1.0f * g_scr_w/g_scr_h);
	viewport1 = new ViewPort(0, 0, g_scr_w, g_scr_h, g_screen);
	renderer1 = new Renderer(scene, camera, viewport1);
	camera->Translate(level->m_things->m_things[0]->m_x/50.0f, 0.0f, level->m_things->m_things[0]->m_y/50.0f);

	while (1)
	{
		VideoWorks();
		int a = RefreshKeybState();
		if (a < 0)
			return;
		else if (a == 1)
			ShowMinimap(level);
		HandleGameplayInput();
	}
	return;
}

Scene* BuildScene(doom::LevelLump * level)
{
	Scene *s = new Scene();
	unsigned int size = (unsigned int) level->m_things->m_things.size();

	for (unsigned int i=0 ; i<size ; i++)
	{
		doom::Thing * thingLump = level->m_things->m_things[i];
		if (thingLump == NULL)
			continue;
		if (thingLump->Load() != 0)
			continue;

		Texture *texture = new Texture(thingLump->m_sprites[0]->m_bitmap,
									   thingLump->m_sprites[0]->m_w,
									   thingLump->m_sprites[0]->m_h);

		Cube *c = new Cube(texture->m_mipmaps[0].m_height / 100.0f, texture);
		c->m_worldmatrix.Translate(thingLump->m_x/50.0f, 0.0f, thingLump->m_y/50.0f);
		s->AddMesh(c);
	}
	return s;
}

void VideoWorks()
{
	static int fpsticks = SDL_GetTicks();

	// Lock surface if needed
	if (SDL_MUSTLOCK(g_screen)) 
		if (SDL_LockSurface(g_screen) < 0) 
			return;

	renderer1->Render();
	
	// Unlock if needed
	if (SDL_MUSTLOCK(g_screen)) 
		SDL_UnlockSurface(g_screen);

	// Tell SDL to update the whole screen
	SDL_UpdateRect(g_screen, 0, 0, g_scr_w, g_scr_h); 
}

int HandleGameplayInput()
{
	static int g_keystick = SDL_GetTicks();
	
	while ( (SDL_GetTicks())-g_keystick > KEYBOARD_RATE_MS ) // Limit the keyrate
	{
		if (g_mouse_x != 0 || g_mouse_y != 0)
		{
			camera->RotateX(-cameraxrotation);
			camera->RotateY(g_mouse_x / 2000.0f);
			cameraxrotation += g_mouse_y / 2000.0f;
			camera->RotateX(cameraxrotation);
			g_mouse_x = 0;
			g_mouse_y = 0;
		}

		if (g_keys['d'])
			camera->RotateY(KEYBOARD_RATE_MS * 3.14159f / 2000.0f);
		if (g_keys['a'])
			camera->RotateY(KEYBOARD_RATE_MS * 3.14159f / -2000.0f);
		if (g_keys['w'])
			camera->RotateX(KEYBOARD_RATE_MS * 3.14159f / 2000.0f);
		if (g_keys['s'])
			camera->RotateX(KEYBOARD_RATE_MS * 3.14159f / -2000.0f);
		if (g_keys['e'])
			camera->RotateZ(KEYBOARD_RATE_MS * 3.14159f / 2000.0f);
		if (g_keys['q'])
			camera->RotateZ(KEYBOARD_RATE_MS * 3.14159f / -2000.0f);

		if (g_keys['i'] || g_keys['k'] || g_keys['j'] || g_keys['l'] || g_keys['o'] || g_keys['u'])
			camera->RotateX(-cameraxrotation);

		if (g_keys['i'])
			camera->Translate(0.0f, 0.0f, KEYBOARD_RATE_MS * 0.004f);
		if (g_keys['k'])
			camera->Translate(0.0f, 0.0f, KEYBOARD_RATE_MS * -0.004f);
		if (g_keys['l'])
			camera->Translate(KEYBOARD_RATE_MS * 0.004f, 0.0f, 0.0f);
		if (g_keys['j'])
			camera->Translate(KEYBOARD_RATE_MS * -0.004f, 0.0f, 0.0f);
		if (g_keys['o'])
			camera->Translate(0.0f, KEYBOARD_RATE_MS * 0.004f, 0.0f);
		if (g_keys['u'])
			camera->Translate(0.0f, KEYBOARD_RATE_MS * -0.004f, 0.0f);

		if (g_keys['i'] || g_keys['k'] || g_keys['j'] || g_keys['l'] || g_keys['o'] || g_keys['u'])
			camera->RotateX(cameraxrotation);

		g_keystick += KEYBOARD_RATE_MS;
	}

	return 0;
}