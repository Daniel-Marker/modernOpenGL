#pragma once

#include "GL/glew.h"
#include "GL\freeglut.h"

class Buffer
{
private:
	GLuint _bufferID;
	GLuint _bufferType;

public:
	Buffer(GLuint bufferType, const void* data, unsigned int size);
	~Buffer();

	void BindBuffer();
};

