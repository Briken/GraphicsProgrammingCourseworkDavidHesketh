#include "stdafx.h"
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
	glBindAttribLocation(m_Program, 1, "texCoord");
	glLinkProgram(m_Program);
	CheckShaderError(m_Program, GL_LINK_STATUS, true, "Error: Shader program link failed");
	glValidateProgram(m_Program);
	CheckShaderError(m_Program, GL_VALIDATE_STATUS, true, "Error: shader program not valid");

	m_Uniforms[TRANSFORM_U] = glGetUniformLocation(m_Program, "transform");
}
void Shader::Bind()
{
	glUseProgram(m_Program);
}


std::string Shader::LoadShader(const std::string & fileName)
{
	std::ifstream shaderFile;
	shaderFile.open((fileName).c_str());
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


Shader::~Shader()
{
	for (int i = 0; i < NUM_SHADERS; i++)
	{
		glDetachShader(m_Program, m_Shaders[i]);
		glDeleteShader(m_Shaders[i]);
	}
	glDeleteProgram(m_Program);
}

void Shader::Update(const Transform& transform, const Camera& camera)
{
	glm::mat4 mvp = camera.GetViewProjection() * transform.GetModel();
	glUniformMatrix4fv(m_Uniforms[TRANSFORM_U], 1, GLU_FALSE, &mvp[0][0]);
}

GLuint Shader::CreateShader(const std::string& text, unsigned int type)
{
	GLuint shader = glCreateShader(type); //create shader based on specified type

	if (shader == 0) //if == 0 shader no created
		std::cerr << "Error type creation failed " << type << std::endl;

	const GLchar* stringSource[1]; //convert strings into list of c-strings
	stringSource[0] = text.c_str();
	GLint lengths[1];
	lengths[0] = text.length();

	glShaderSource(shader, 1, stringSource, lengths); //send source code to opengl
	glCompileShader(shader); //get open gl to compile shader code

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!"); //check for compile error

	return shader;
}
