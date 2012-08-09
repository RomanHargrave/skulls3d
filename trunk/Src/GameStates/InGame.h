
#ifndef SK_INGAME
#define SK_INGAME

#include <windows.h>
#include <list>

#include "GameState.h"
#include "Mesh.h"
#include "Scene.h"
#include "Camera.h"
#include "doom/Vertex.h"

class InGame : public GameState
{
	doom::LevelLump * m_level;
	Scene *m_scene;
	Camera *m_camera;
	
public:
	InGame(doom::LevelLump * level);

	virtual void Render();
	virtual LRESULT WINAPI HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	Mesh* CreateMesh(doom::Vertex v0, doom::Vertex v1, int low, int high, Tex *tex);
};

#endif // SK_INGAME