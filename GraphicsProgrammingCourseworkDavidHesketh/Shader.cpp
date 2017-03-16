#include "Shader.h"
#include <iostream>
#include <fstream>

Shader::Shader(const std::string& filename)
{
	m_Program = glCreateProgram();
	m_Shaders[0] = CreateShader(LoadShader(filename + ".vert"), GL_VERTEX_SHADER);
	m_Shaders[1] = CreateShader(LoadShader(filename + ".frag"), GL_FRAGMENT_SHADER);

	for (int i = 0; i < NUM_SHADERS; i++)
	{
		glAttachShader(m_Program, m_Shaders[i]);
	}
	glBindAttribLocation(m_Program, 0, "position");
	glLinkProgram(m_Program);
	CheckShaderError(m_Program, GL_LINK_STATUS, true, "Error: Shader program link failed");
	glValidateProgram(m_Program);
	CheckShaderError(m_Program, GL_VALIDATE_STATUS, true, "Error: shader program not valid");
}
void Shader::Bind()
{
	glUseProgram(m_Program);
}


std::string Shader::LoadShader(const std::string & fileName)
{
	std::ifstream shaderFile;
	shaderFile.open((fileName).c_str);
	std::string output;
	std::string line;

	if (shaderFile.is_open())
	{
		while (shaderFile.good())
		{
			getline(shaderFile, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return output;
}

void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string & errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };
	if (success == GL_FALSE)
	{

		if (isProgram)
		{
			glGetProgramiv(shader, flag, &success);
		}
		else
		{
			glGetShaderiv(shader, flag, &success);
		}
		std::cerr << errorMessage << ": " << error << "" << std::endl;
	}
}

GLuint Shader::CreateShader(const std::string & text, unsigned int type)
{
	return GLuint();
}


Shader::~Shader()
{
	for (int i = 0; i < NUM_SHADERS; i++)
	{
		glDetachShader(m_Program, m_Shaders[i]);
		glDeleteShader(m_Shaders[i]);
	}
	glDeleteProgram(m_Program);
}
