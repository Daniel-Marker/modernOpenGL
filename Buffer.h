#pragma once

#include "GL/glew.h"
#include "GL\freeglut.h"

class Buffer
{
private:
	GLuint _bufferID;
	GLenum _bufferType;

public:
	Buffer(GLenum bufferType, const void* data, unsigned int size);
	~Buffer();

	void BindBuffer();
};

