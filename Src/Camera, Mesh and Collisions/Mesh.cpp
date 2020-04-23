#include "Mesh.h"
#include "Common.cpp"

bool Mesh::LoadFromFile(std::string path)
{
	std::string extension = GetFileExtension(path);
	std::ifstream inFile(path, std::ios::in);
	std::string currentLine = "";

	glm::vec3* tempVertexPositions;
	int tempVertexCount;
	glm::vec2* tempUVCoords;
	int tempUVCount;
	glm::vec3* tempVertexNormals;
	int tempNormalCount;
	int tempFaceCount;
	int faceStart = 0;

	if (!inFile.is_open())
	{
		std::cout << "Failed to open " << path << std::endl;
		return false;
	}

	if (extension != "obj")
	{
		std::cout << "Failed to load " << path << " as it is not a .obj file" << std::endl;
		return false;
	}

	tempVertexPositions = LoadPositionData(path, inFile, tempVertexCount);
	tempUVCoords = LoadUVCoordData(path, inFile, tempUVCount);
	tempVertexNormals = LoadNormalData(path, inFile, tempNormalCount);

	//if any == nullptr, then it failed to load one of the following, so return false
	if (tempVertexPositions == nullptr || tempUVCoords == nullptr || tempVertexNormals == nullptr)
	{
		return false;
	}

	tempFaceCount = LoadFaceCount(path, inFile, faceStart);

	//no of indices = no of faces * 3, as each face has 3 vertices
	_indexCount = tempFaceCount * 3;
	_indices = new unsigned int[_indexCount];

	_vertexCount = tempFaceCount * 3;
	_vertexPositions = new glm::vec3[_vertexCount];

	//each vertex has to have a uv coord, so no uv coords = vertexCount
	_uvCount = _vertexCount;
	_uvCoords = new glm::vec2[_uvCount];

	//each vertex has to have a normal, so normal count = vertex count
	_normalCount = _vertexCount;
	_vertexNormals = new glm::vec3[_normalCount];

	inFile.close();
	inFile.open(path, std::ios::in);	//For some reason, trying to read more from the file here didn't work. But reopening it fixed it
	inFile.seekg(std::streampos(faceStart));
	int indexNumber = 0;
	for (int i = 0; i < tempFaceCount; i++)
	{
		std::string lineType;
		int vertexIndex, uvIndex, normIndex;
		char slash;

		inFile >> lineType;
		for (int j = 0; j < 3; j++)
		{
			inFile >> vertexIndex;
			inFile >> slash;			//Read in the slash between values

			inFile >> uvIndex;
			inFile >> slash;

			inFile >> normIndex;

			_vertexPositions[3*i + j] = tempVertexPositions[vertexIndex - 1];
			_uvCoords[3 * i + j] = tempUVCoords[uvIndex - 1];
			_vertexNormals[3 * i + j] = tempVertexNormals[normIndex - 1];
			_indices[3 * i + j] = 3 * i + j;			
		}

		std::getline(inFile, currentLine);
		if (currentLine != "")
		{
			std::cerr << "Mesh has non-triangle faces. Please triangulate your mesh before importing" << std::endl;
			return false;
		}
	}

	inFile.close();
	delete[] tempVertexPositions;
	delete[] tempUVCoords;
	return true;
}

glm::vec3* Mesh::LoadPositionData(std::string path, std::ifstream& inFile, int& tempVertexCount)
{
	glm::vec3* tempVertexPositions = nullptr;
	std::string currentLine = "";
	int vertexStart = 0;
	//vertexStart is used to get the position in the file, as tellg() kept returning a position a few lines above where it last read


	while (currentLine.substr(0, 2) != "v " && !inFile.eof())
	{
		std::getline(inFile, currentLine);
		vertexStart += currentLine.length() + 1;	//add the no of characters from the line
	}
	if (inFile.eof())
	{
		std::cout << path << " has no vertices" << std::endl;
		return tempVertexPositions;
	}
	vertexStart -= currentLine.length() + 1;		//subtract so that we are at the start of the first vertex


	tempVertexCount = 0;
	while (currentLine.substr(0, 2) == "v ")
	{
		tempVertexCount++;
		std::getline(inFile, currentLine);
	}
	tempVertexPositions = new glm::vec3[tempVertexCount];


	inFile.seekg(std::streampos(vertexStart));
	for (int i = 0; i < tempVertexCount; i++)
	{
		std::string lineType;
		float position;
		inFile >> lineType;

		inFile >> position;
		tempVertexPositions[i].x = position;
		inFile >> position;
		tempVertexPositions[i].y = position;
		inFile >> position;
		tempVertexPositions[i].z = position;
	}
	return tempVertexPositions;
}

glm::vec2* Mesh::LoadUVCoordData(std::string path, std::ifstream& inFile, int& tempUVCount)
{
	glm::vec2* tempUVPositions = nullptr;
	std::string currentLine = "";
	int uvStart = 0;
	//tellg doesn't seem to give correct position, so we have to go back to the start of the file and start looking again

	inFile.close();
	inFile.open(path);	//For some reason, trying to read more from the file here didn't work. But reopening it fixed it

	while (currentLine.substr(0, 2) != "vt" && !inFile.eof())
	{
		std::getline(inFile, currentLine);
		uvStart += currentLine.length() + 1;	//add the no of characters from the line
	}
	if (inFile.eof())
	{
		std::cout << path << " has no uv coordinates" << std::endl;
		return tempUVPositions;
	}
	uvStart -= currentLine.length() + 1;		//subtract so that we are at the start of the first uv coordinate


	tempUVCount = 0;
	while (currentLine.substr(0, 2) == "vt")
	{
		tempUVCount++;
		std::getline(inFile, currentLine);
	}
	tempUVPositions = new glm::vec2[tempUVCount];


	inFile.seekg(std::streampos(uvStart));
	for (int i = 0; i < tempUVCount; i++)
	{
		std::string lineType;
		float position;
		inFile >> lineType;

		inFile >> position;
		tempUVPositions[i].x = position;
		inFile >> position;
		tempUVPositions[i].y = position;
	}

	return tempUVPositions;
}

glm::vec3* Mesh::LoadNormalData(std::string path, std::ifstream& inFile, int& tempNormalCount)
{
	glm::vec3* tempVertexNormals = nullptr;
	std::string currentLine = "";
	int normalStart = 0;
	//tellg doesn't seem to give correct position, so we have to go back to the start of the file and start looking again

	inFile.close();
	inFile.open(path);	//For some reason, trying to read more from the file here didn't work. But reopening it fixed it

	while (currentLine.substr(0, 2) != "vn" && !inFile.eof())
	{
		std::getline(inFile, currentLine);
		normalStart += currentLine.length() + 1;	//add the no of characters from the line
	}
	if (inFile.eof())
	{
		std::cout << path << " has no vertex normals" << std::endl;
		return tempVertexNormals;
	}
	normalStart -= currentLine.length() + 1;		//subtract so that we are at the start of the first vertex normal


	tempNormalCount = 0;
	while (currentLine.substr(0, 2) == "vn")
	{
		tempNormalCount++;
		std::getline(inFile, currentLine);
	}
	tempVertexNormals = new glm::vec3[tempNormalCount];


	inFile.seekg(std::streampos(normalStart));
	for (int i = 0; i < tempNormalCount; i++)
	{
		std::string lineType;
		float position;
		inFile >> lineType;

		inFile >> position;
		tempVertexNormals[i].x = position;
		inFile >> position;
		tempVertexNormals[i].y = position;
		inFile >> position;
		tempVertexNormals[i].z = position;
	}

	return tempVertexNormals;
}

int Mesh::LoadFaceCount(std::string path, std::ifstream& inFile, int& faceStart)
{
	std::string currentLine = "";
	int tempFaceCount;

	inFile.close();
	inFile.open(path, std::ios::in);	//For some reason, trying to read more from the file here didn't work. But reopening it fixed it
	while (currentLine.substr(0, 2) != "f " && !inFile.eof())
	{
		std::getline(inFile, currentLine);
		faceStart += currentLine.length() + 1;	//add the no of characters from the line
	}
	if (inFile.eof())
	{
		std::cerr << "Obj file has no faces " << std::endl;
		return false;
	}
	faceStart -= currentLine.length() + 1;		//subtract so that we are at the start of the first face


	tempFaceCount = 0;
	while (currentLine.substr(0, 2) == "f ")
	{
		tempFaceCount++;
		std::getline(inFile, currentLine);
	}

	return tempFaceCount;
}

Mesh::Mesh(std::string path)
{
	if (!LoadFromFile(path))
		std::cerr << "Failed to load " << path << std::endl;
	else
		std::cout << path << " loaded ok" << std::endl;
}

Mesh::~Mesh()
{
	delete[] _vertexPositions;
	delete[] _uvCoords;
	delete[] _vertexNormals;
	delete[] _indices;
}
