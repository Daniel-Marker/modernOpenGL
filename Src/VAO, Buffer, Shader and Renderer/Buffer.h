#pragma once

#include "GL/glew.h"
#include "GL\freeglut.h"

class Buffer
{
private:
	GLuint _bufferID;
	GLenum _bufferType;
	int _size;

public:
	Buffer(GLenum bufferType, const void* data, unsigned int size);
	Buffer(GLenum bufferType, unsigned int size);
	~Buffer();

	int GetSize() { return _size; };

	void BindBuffer();
	void UpdateBuffer(int offset, int size, void* data);
};

