#pragma once

#include "GL/glew.h"
#include "GL\freeglut.h"

#include "Buffer.h"
#include "BufferLayout.h"
#include "Mesh.h"

class Vao
{
private:
	GLuint _vaoID;

	Buffer* _vertexBuffer;
	Buffer* _indexBuffer;

public:
	Vao();
	~Vao();

	void CreateIndexBuffer(Mesh* mesh);
	void CreateIndexBuffer(void* indexData, int size);
	
	void CreateVertexBuffer(Mesh* mesh, BufferLayout layout);
	void CreateVertexBuffer(void* vertexData, int size, BufferLayout layout);
	
	void UpdateVertexBuffer(int offset, int size, void* data);

	Buffer* const GetIndexBuffer(){ return _indexBuffer; };

	void BindVao();
};

