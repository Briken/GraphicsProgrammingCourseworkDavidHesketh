#pragma once
#include "stdafx.h"
#include <SDL\SDL.h>
#include <GL\glew.h>
#include <iostream>
#include <string>
class Window
{
public:
	Window();
	~Window();

	void initWindow();
	void swapBuffer();
	void clearWindow(float r, float g, float b, float a);

	float getWidth();
	float getHeight();
private:
	void ReturnError(std::string errorString);
	
	
	SDL_GLContext gameContext; //global variable to hold the context
	SDL_Window* gameWindow; //holds pointer to out window
	float screenWidth;
	float screenHeight;
};

