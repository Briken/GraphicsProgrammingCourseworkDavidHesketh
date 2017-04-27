#pragma once

#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Screen.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"

enum class GameState { PLAY, EXIT };

class GameApplication
{
public:
	GameApplication();
	~GameApplication();

	void Run();

private:

	void InitSystems();
	void ProcessInput();
	void GameLoop();
	void DrawGame();

	Screen m_GameDisplay;
	GameState m_GameState;

	Mesh object1;
	Mesh object2;
	Camera gameCamera;

	float counter;
};

