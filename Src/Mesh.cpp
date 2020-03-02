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

	tempVertexPositions = LoadPositionData(inFile, tempVertexCount);
	tempUVCoords = LoadUVCoordData(inFile, tempUVCount);
	if (tempVertexPositions == nullptr || tempUVCoords == nullptr)
	{
		std::cout << "Failed to load " << path << std::endl;
		return false;
	}

	inFile.seekg(inFile.beg);
	//This should be able to handle if there is no "f " found
	while (currentLine.substr(0, 2) != "f ")
	{
		std::getline(inFile, currentLine);
		faceStart += currentLine.length() + 1;	//add the no of characters from the line
	}
	faceStart -= currentLine.length() + 1;		//subtract so that we are at the start of the first face


	tempFaceCount = 0;
	while (currentLine.substr(0, 2) == "f ")
	{
		tempFaceCount++;
		std::getline(inFile, currentLine);
	}

	//no of indices = no of faces * 3, as each face has 3 vertices
	_indexCount = tempFaceCount * 3;
	_indices = new unsigned int[_indexCount];

	//each vertex has to be repeated 3 times to give faces unique uv coords, so vertexCount = 3 * no of vertices in file
	_vertexCount = tempFaceCount * 3;
	_vertexPositions = new glm::vec3[_vertexCount];

	//each vertex has to have a uv coord, so no uv coords = vertexCount
	_uvCount = _vertexCount;
	_uvCoords = new glm::vec2[_uvCount];

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
			_indices[3 * i + j] = 3 * i + j;			
		}

		std::getline(inFile, currentLine);
	}

	inFile.close();
	delete[] tempVertexPositions;
	delete[] tempUVCoords;
	return true;
}

glm::vec3* Mesh::LoadPositionData(std::ifstream& inFile, int& tempVertexCount)
{
	glm::vec3* tempVertexPositions = nullptr;
	std::string currentLine = "";
	int vertexStart = 0;
	//vertexStart is used to get the position in the file, as tellg() kept returning a position a few lines above where it last read


	//This should handle if there is no 'v' found
	while (currentLine.substr(0, 2) != "v ")
	{
		std::getline(inFile, currentLine);
		vertexStart += currentLine.length() + 1;	//add the no of characters from the line
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

glm::vec2* Mesh::LoadUVCoordData(std::ifstream& inFile, int& tempUVCount)
{
	glm::vec2* tempUVPositions = nullptr;
	std::string currentLine = "";
	int uvStart = 0;
	//tellg doesn't seem to give correct position, so we have to go back to the start of the file and start looking again

	inFile.seekg(inFile.beg);
	//This should handle if there is no "vt " found
	while (currentLine.substr(0, 2) != "vt")
	{
		std::getline(inFile, currentLine);
		uvStart += currentLine.length() + 1;	//add the no of characters from the line
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

Mesh::Mesh(std::string path)
{
	if (!LoadFromFile(path))
		std::cerr << "Failed to load " << path << std::endl;
}

Mesh::~Mesh()
{
	delete[] _vertexPositions;
	delete[] _uvCoords;
	delete[] _indices;
}
