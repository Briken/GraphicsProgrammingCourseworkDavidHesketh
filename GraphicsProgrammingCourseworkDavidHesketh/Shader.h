#pragma once
#include <string>
#include <GL\glew.h>

class Shader
{
public:
	Shader(const std::string& filename);

	void Bind();
	std::string LoadShader(const std::string& filename);
	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	GLuint CreateShader(const std::string& text, unsigned int type);

	~Shader();
protected:
private:
	static const int NUM_SHADERS = 2;
	Shader(const Shader& other) {}
	void operator= (const Shader& other) {}

	GLuint m_Program;
	GLuint m_Shaders[NUM_SHADERS];

};

