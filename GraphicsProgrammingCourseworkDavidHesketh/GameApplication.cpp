#include "stdafx.h"
#include "GameApplication.h"
#include "Camera.h"
#include <iostream>
#include <string>

Transform objectTransform;

GameApplication::GameApplication()
{
	m_GameState = GameState::PLAY;
	Screen* m_GameDisplay = new Screen();
	Mesh* object1();
	Mesh* object2();
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

	
	
	Shader shader("res/shaders/shader");
	Texture texture("C:\\Users\\broke\\Downloads\\Lab4\\Lab4\\res\\bricks.jpg"); //load texture
	Texture texture1("C:\\Users\\broke\\Downloads\\Lab4\\Lab4\\res\\water.jpg"); //load texture
	
	
	objectTransform.SetPos(glm::vec3(sinf(counter), 0.0, 0.0));
	objectTransform.SetRot(glm::vec3(0.0, 0.0, counter * 5));
	objectTransform.SetScale(glm::vec3(sinf(counter), sinf(counter), sinf(counter)));

	shader.Bind();
	shader.Update(objectTransform, gameCamera);
	texture.Bind(0);
	object2.Draw();
	counter += 0.01f;

	glEnableClientState(GL_COLOR_ARRAY);
	glEnd();

	m_GameDisplay.SwapBuffer();
}
