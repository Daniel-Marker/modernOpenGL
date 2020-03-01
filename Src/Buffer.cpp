#include "Buffer.h"

//Code for this class is based off of code from videos 1-26 in this series: https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2

Buffer::Buffer(GLenum bufferType, const void* data, unsigned int size):
	_bufferType(bufferType), _size(size)
{
	glGenBuffers(1, &_bufferID);
	BindBuffer();
	glBufferData(_bufferType, size, data, GL_STATIC_DRAW);
}

Buffer::Buffer(GLenum bufferType, unsigned int size):
	_bufferType(bufferType), _size(size)
{
	glGenBuffers(1, &_bufferID);
	BindBuffer();
	glBufferData(_bufferType, _size, 0, GL_STATIC_DRAW);
}

Buffer::~Buffer()
{
	glDeleteBuffers(1, &_bufferID);
}

void Buffer::BindBuffer()
{
	glBindBuffer(_bufferType, _bufferID);
}

void Buffer::UpdateBuffer(int offset, int size, void* data)
{
	BindBuffer();
	glBufferSubData(_bufferType, offset, size, data);
}
