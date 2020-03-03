#include "Vao.h"

//Code for this class is based off of code from videos 1-26 in this series: https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2

Vao::Vao():
	_indexBuffer(nullptr), _vertexBuffer(nullptr)
{
	glGenVertexArrays(1, &_vaoID);
}

Vao::~Vao()
{
	glDeleteVertexArrays(1, &_vaoID);

	delete _vertexBuffer;
	delete _indexBuffer;
}

void Vao::CreateIndexBuffer(Mesh* mesh)
{
	if (_indexBuffer != nullptr)
		delete _indexBuffer;

	BindVao();

	_indexBuffer = new Buffer(GL_ELEMENT_ARRAY_BUFFER, mesh->GetIndices(), mesh->GetIndexCount() * sizeof(unsigned int));
}

void Vao::CreateVertexBuffer(Mesh* mesh, BufferLayout layout)
{
	if (_vertexBuffer != nullptr)
		delete _vertexBuffer;

	BindVao();

	unsigned int sizeOfVertexData = mesh->GetVertexCount() * sizeof(glm::vec3);
	unsigned int sizeOfUVData = mesh->GetUVCount() * sizeof(glm::vec2);
	unsigned int sizeOfNormalData = mesh->GetVertexNormalCount() * sizeof(glm::vec3);

	_vertexBuffer = new Buffer(GL_ARRAY_BUFFER, sizeOfVertexData + sizeOfUVData + sizeOfNormalData);
	_vertexBuffer->UpdateBuffer(0, sizeOfVertexData, mesh->GetVertexPositions());
	_vertexBuffer->UpdateBuffer(sizeOfVertexData, sizeOfUVData, mesh->GetUVCoords());
	_vertexBuffer->UpdateBuffer(sizeOfVertexData + sizeOfUVData, sizeOfNormalData, mesh->GetVertexNormals());

	std::vector<layoutElement> bufferLayout = layout.GetLayout();
	int offset = 0;

	for (int i = 0; i < bufferLayout.size(); i++)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, bufferLayout[i]._size, bufferLayout[i]._type, bufferLayout[i]._normalized, 0,(void*)offset);

		offset += bufferLayout[i]._size * bufferLayout[i].GetSizeOfType() * bufferLayout[i]._count;
	}
}

void Vao::BindVao()
{
	glBindVertexArray(_vaoID);
}
