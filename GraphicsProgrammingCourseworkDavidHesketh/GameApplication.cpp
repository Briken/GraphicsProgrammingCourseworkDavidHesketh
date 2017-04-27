#include "stdafx.h"
#include "GameApplication.h"


GameApplication::GameApplication()
{
	m_GameState = GameState::PLAY;
	Screen* m_GameDisplay = new Screen();
}


GameApplication::~GameApplication()
{
}

void GameApplication::Run()
{
	InitSystems();
	GameLoop();
}

void GameApplication::InitSystems()
{
	m_GameDisplay.InitDisplay();
}

void GameApplication::ProcessInput()
{
	SDL_Event evnt;

	while (SDL_PollEvent(&evnt)) //get and process events
	{
		switch (evnt.type)
		{
		case SDL_QUIT:
			m_GameState = GameState::EXIT;
			break;
		}
	}
}

void GameApplication::GameLoop()
{
	while (m_GameState != GameState::EXIT)
	{
		ProcessInput();
		DrawGame();
	}
}

void GameApplication::DrawGame()
{
	m_GameDisplay.ClearDisplay();

	Vertex vertArray[] = {
		Vertex(glm::vec3(-0.5f,-0.5f,-0)),
		Vertex(glm::vec3(0,-0.5f,0)),
		Vertex(glm::vec3(0.5, -0.5f, 0))
	};
	Mesh objMesh(vertArray, sizeof(vertArray) / sizeof(vertArray[0]));
	Shader shader("res/shaders/shader");
	shader.Bind();
	objMesh.Draw();

	// old code for testing only 
	glEnableClientState(GL_COLOR_ARRAY);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(0, 0);
	glVertex2f(0, 500);
	glVertex2f(500, 500);
	glEnd();

	m_GameDisplay.SwapBuffer();
}
