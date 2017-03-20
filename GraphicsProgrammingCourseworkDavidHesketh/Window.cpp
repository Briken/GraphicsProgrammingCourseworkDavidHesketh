#include "Window.h"
Window::Window()
{
	gameWindow = nullptr; //initialise to generate null access violation for debugging. 
	screenWidth = 1024.0f;
	screenHeight = 768.0f;
}

Window::~Window()
{
	SDL_GL_DeleteContext(gameContext); // delete context
	SDL_DestroyWindow(gameWindow); // detete window (make sure to delete the context and the window in the opposite order of creation in initDisplay())
	SDL_Quit();
}

float Window::getWidth() { return screenWidth; } //getters
float Window::getHeight() { return screenHeight; }

void Window::ReturnError(std::string errorString)
{
	std::cout << errorString << std::endl;
	std::cout << "press any  key to quit...";
	int in;
	std::cin >> in;
	SDL_Quit();
}

void Window::swapBuffer()
{
	SDL_GL_SwapWindow(gameWindow); //swap buffers
}

void Window::clearWindow(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear colour and depth buffer - set colour to colour defined in glClearColor
}

void Window::initWindow()
{
	SDL_Init(SDL_INIT_EVERYTHING); //initalise everything

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); //Min no of bits used to diplay colour
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);// set up z-buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // set up double buffer   

	gameWindow = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)screenWidth, (int)screenHeight, SDL_WINDOW_OPENGL); // create window

	if (gameWindow == nullptr)
	{
		ReturnError("window failed to create");
	}

	gameContext = SDL_GL_CreateContext(gameWindow);

	if (gameContext == nullptr)
	{
		ReturnError("SDL_GL context failed to create");
	}

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		ReturnError("GLEW failed to initialise");
	}

	glEnable(GL_DEPTH_TEST); //enable z-buffering 
	glEnable(GL_CULL_FACE); //dont draw faces that are not pointing at the camera

	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
}