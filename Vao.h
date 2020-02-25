#pragma once

#include "GL/glew.h"
#include "GL\freeglut.h"

#include "Buffer.h"

class Vao
{
private:
	GLuint _vaoID;

	Buffer* _vertexBuffer;
	Buffer* _indexBuffer;

public:
	Vao();
	~Vao();

	void CreateIndexBuffer(const void* data, unsigned int size);
	void CreateVertexBuffer(const void* data, unsigned int size);

	void BindVao();
};

