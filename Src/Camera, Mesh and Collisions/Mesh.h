#pragma once

#include <fstream>
#include <iostream>
#include <string>

#include "glm/glm.hpp"

class Mesh
{
private:
	glm::vec3* _vertexPositions;
	glm::vec2* _uvCoords;
	glm::vec3* _vertexNormals;
	unsigned int* _indices;

	int _vertexCount, _uvCount, _indexCount, _normalCount;

	bool LoadFromFile(std::string path);
	glm::vec3* LoadPositionData(std::string path, std::ifstream& inFile, int& tempVertexCount);
	glm::vec2* LoadUVCoordData(std::string path, std::ifstream& inFile, int& tempUVCount);
	glm::vec3* LoadNormalData(std::string path, std::ifstream& inFile, int& tempNormalCount);
	int LoadFaceCount(std::string path, std::ifstream& inFile, int& faceStart);

public:
	Mesh(std::string path);
	~Mesh();

	glm::vec3* const GetVertexPositions() { return _vertexPositions; }
	int GetVertexCount() { return _vertexCount; }

	glm::vec2* const GetUVCoords() { return _uvCoords; }
	int GetUVCount() { return _uvCount; }

	glm::vec3* const GetVertexNormals() { return _vertexNormals; }
	int GetVertexNormalCount() { return _normalCount; }

	unsigned int* const GetIndices() { return _indices; }
	int GetIndexCount() { return _indexCount; }
};

