#include "Vao.h"

Vao::Vao():
	_indexBuffer(nullptr), _vertexBuffer(nullptr)
{
	glGenVertexArrays(1, &_vaoID);
}

Vao::~Vao()
{
	glDeleteVertexArrays(1, &_vaoID);
}

void Vao::CreateIndexBuffer(const void* data, unsigned int size)
{
	if (_indexBuffer != nullptr)
		delete _indexBuffer;

	BindVao();

	_indexBuffer = new Buffer(GL_ELEMENT_ARRAY_BUFFER, data, size);
}

void Vao::CreateVertexBuffer(const void* data, unsigned int size, BufferLayout layout)
{
	if (_vertexBuffer != nullptr)
		delete _vertexBuffer;

	BindVao();

	_vertexBuffer = new Buffer(GL_ARRAY_BUFFER, data, size);

	std::vector<layoutElement> bufferLayout = layout.GetLayout();

	for (int i = 0; i < bufferLayout.size(); i++)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, bufferLayout[i]._size, bufferLayout[i]._type, bufferLayout[i]._normalized, bufferLayout[i]._stride, nullptr);

	}
}

void Vao::BindVao()
{
	glBindVertexArray(_vaoID);
}
