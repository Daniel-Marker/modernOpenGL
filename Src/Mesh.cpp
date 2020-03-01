#include "Mesh.h"

static float vertexData[] =
{
	//Positions
	1, 1, 1,
	-1, 1, 1,
	-1, -1, 1,
	1, -1, 1,
	1, 1, 1,
	1, -1, 1,
	1, -1, -1,
	1, 1, -1,
	1, 1, 1,
	1, 1, -1,
	-1, 1, -1,
	-1, 1, 1,
	-1, 1, 1,
	-1, 1, -1,
	-1, -1, -1,
	-1, -1, 1,
	-1, -1, -1,
	1, -1, -1,
	1, -1, 1,
	-1, -1, 1,
	1, -1, -1,
	-1, -1, -1,
	-1, 1, -1,
	1, 1, -1,

	//UV coords
	1, 0,
	1, 1,
	0, 1,
	0, 0,
	1, 0,
	1, 1,
	0, 1,
	0, 0,
	1, 0,
	1, 1,
	0, 1,
	0, 0,
	1, 0,
	1, 1,
	0, 1,
	0, 0,
	1, 0,
	1, 1,
	0, 1,
	0, 0,
	1, 0,
	1, 1,
	0, 1,
	0, 0
};

static unsigned int indices[] =
{
	0, 1, 2,
	2, 3, 0,
	4, 5, 6,
	6, 7, 4,
	8, 9, 10,
	10, 11, 8,
	12, 13, 14,
	14, 15, 12,
	16, 17, 18,
	18, 19, 16,
	20, 21, 22,
	22, 23, 20
};

Mesh::Mesh()
{
	_vertexCount = 24;
	_uvCount = 24;
	_indexCount = 36;

	_vertexPositions = new glm::vec3[_vertexCount];
	_uvCoords = new glm::vec2[_uvCount];
	_indices = new unsigned int[_indexCount];

	for (int i = 0; i < _vertexCount; i++)
	{
		_vertexPositions[i].x = vertexData[i * 3];
		_vertexPositions[i].y = vertexData[i * 3 + 1];
		_vertexPositions[i].z = vertexData[i * 3 + 2];

		_uvCoords[i].x = vertexData[i * 2 + 24 * 3];
		_uvCoords[i].y = vertexData[i * 2 + 24 * 3 + 1];
	}

	for (unsigned int i = 0; i < _indexCount; i++)
	{
		_indices[i] = indices[i];
	}
}

Mesh::~Mesh()
{
	delete[] _vertexPositions;
	delete[] _uvCoords;
}
