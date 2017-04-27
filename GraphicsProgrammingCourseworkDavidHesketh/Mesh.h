#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>
#include "ModelLoader.h"

class Vertex
{
public:
	Vertex(const glm::vec3& pos, const glm::vec2& texCoord)
	{
		this->m_Position = pos;
		this->m_TextureCoord = texCoord;
		this->m_Normal = m_Normal;
	}

	glm::vec3* GetPosition() { return &m_Position; }
	glm::vec2* GetTextureCoord() { return &m_TextureCoord; }
	glm::vec3* GetNormal() { return &m_Normal; }

protected:
private:
	glm::vec3 m_Position;
	glm::vec2 m_TextureCoord;
	glm::vec3 m_Normal;
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	void Draw();
	void InitMesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	void loadModel(const std::string& filename);
	void InitModel(const IndexedModel& model);

private:

	enum
	{
		POSITION_VERTEXBUFFER,
		TEXCOORD_VB,
		NORMAL_VB,
		INDEX_VB,
		NUM_BUFFERS
	};

	GLuint m_VertexArrayObject;
	GLuint m_VertexArrayBuffers[NUM_BUFFERS];
	unsigned int m_DrawCount;
};

