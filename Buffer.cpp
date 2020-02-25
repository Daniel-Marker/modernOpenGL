#include "Buffer.h"

Buffer::Buffer(GLuint bufferType, const void* data, unsigned int size):
	_bufferType(bufferType)
{
	glGenBuffers(1, &_bufferID);
	BindBuffer();
	glBufferData(_bufferType, size, data, GL_STATIC_DRAW);
}

Buffer::~Buffer()
{
	glDeleteBuffers(1, &_bufferID);
}

void Buffer::BindBuffer()
{
	glBindBuffer(_bufferType, _bufferID);
}