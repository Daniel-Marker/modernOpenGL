#pragma once

#include "glm/glm.hpp"

class Mesh
{
private:
	glm::vec3* _vertexPositions;
	glm::vec2* _uvCoords;
	unsigned int* _indices;

	int _vertexCount, _uvCount, _indexCount;

public:
	Mesh();
	~Mesh();

	glm::vec3* const GetVertexPositions() { return _vertexPositions; }
	int GetVertexCount() { return _vertexCount; }

	glm::vec2* const GetUVCoords() { return _uvCoords; }
	int GetUVCount() { return _uvCount; }

	unsigned int* const GetIndices() { return _indices; }
	int GetIndexCount() { return _indexCount; }
};

