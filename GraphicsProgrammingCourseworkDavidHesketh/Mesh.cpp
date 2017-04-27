#include "stdafx.h"
#include "Mesh.h"


Mesh::Mesh(Vertex* vertices, unsigned int numVertices)
{
	m_DrawCount = numVertices;

	glGenVertexArrays(1, &m_VertexArrayObject);
	glBindVertexArray(m_VertexArrayObject);
	glGenBuffers(NUM_BUFFERS, m_VertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexArrayBuffers[POSTITON_VERTEXBUFFER]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
}


Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_VertexArrayObject);
}

void Mesh::Draw()
{
	glBindVertexArray(m_VertexArrayObject);
	glDrawArrays(GL_TRIANGLES, 0, m_DrawCount);
	glBindVertexArray(0);
}

Vertex::Vertex(const glm::vec3 & pos)
{
}
