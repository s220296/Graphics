#include "Mesh.h"
#include <gl_core_4_4.h>

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void Mesh::initialiseQuad()
{
	// check that the mesh is not initialized already
	assert(vao == 0);

	// generate buffers
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	// bind vertex array aka a mesh wrapper
	glBindVertexArray(vao);

	// bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// define 6 vertices for 2 triangles
	Vertex vertices[6];
	vertices[0].position = { -0.5f, 0, 0.5f, 1 };
	vertices[1].position = { 0.5f, 0, 0.5f, 1 };
	vertices[2].position = { -0.5f, 0, -0.5f, 1 };

	vertices[3].position = { -0.5f, 0, -0.5f, 1 };
	vertices[4].position = { 0.5f, 0, 0.5f, 1 };
	vertices[5].position = { 0.5f, 0, -0.5f, 1 };

	// fill vertex buffer
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	// enable first element as position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	
	// unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// quad has 2 triangles
	triCount = 2;
}

void Mesh::draw()
{
}
