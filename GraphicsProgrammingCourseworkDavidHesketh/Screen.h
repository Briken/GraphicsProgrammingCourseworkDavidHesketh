#pragma once
#include <SDL/SDL.h>
#include <GL\glew.h>
#include <iostream>
#include <string>
using namespace std;

class Screen
{
public:
	SDL_GLContext gameContext;

	Screen();
	~Screen();

	void InitDisplay();
	void SwapBuffer();
	void ClearDisplay();

private:

	void ReturnError(std::string errorString);

	SDL_Window* _window; //holds pointer to out window
	int _screenWidth;
	int _screenHeight;
};

