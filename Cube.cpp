#include "Cube.h"

Vertex* Cube::indexedVertices = nullptr;
Color* Cube::indexedColors = nullptr;
GLushort* Cube::indices = nullptr;
int Cube::numVertices = 0;
int Cube::numColors = 0;
int Cube::numIndices = 0;

Cube::Cube(float xPos, float yPos, float zPos, float xAngle, float yAngle, float zAngle, float rotSpeed) :
	_rotation(xAngle, yAngle, zAngle), _rotationSpeed(rotSpeed)
{
	_position.x = xPos;
	_position.y = yPos;
	_position.z = zPos;
}


Cube::~Cube()
{
}

void Cube::Draw()
{
	if (indexedVertices != nullptr && indexedColors != nullptr && indices != nullptr) {
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, indexedVertices);
		glColorPointer(3, GL_FLOAT, 0, indexedColors);

		glPushMatrix();

		glTranslatef(_position.x, _position.y, _position.z);

		glRotatef(_rotation.x, 1.0f, 0.0f, 0.0f);
		glRotatef(_rotation.y, 0.0f, 1.0f, 0.0f);
		glRotatef(_rotation.z, 0.0f, 0.0f, 1.0f);

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, indices);
		glPopMatrix();

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}

void Cube::Update()
{
	_rotation.y += _rotationSpeed;
	_rotation.z += _rotationSpeed;

	/*_position.z -= 0.5f;

	if (_position.z <= 0.1f)
		_position.z = 100.0f;*/
}

bool Cube::Load(char* path)
{
	std::ifstream inFile;
	inFile.open(path);
	if (!inFile.good())
	{
		std::cerr << "Can't open text file " << path << std::endl;
		return false;
	}

	inFile >> numVertices;
	indexedVertices = new Vertex[numVertices];
	for (int i = 0; i < numVertices; i++)
	{
		inFile >> indexedVertices[i].x;
		inFile >> indexedVertices[i].y;
		inFile >> indexedVertices[i].z;
	}

	inFile >> numColors;
	indexedColors = new Color[numColors];
	for (int i = 0; i < numColors; i++)
	{
		inFile >> indexedColors[i].r;
		inFile >> indexedColors[i].g;
		inFile >> indexedColors[i].b;
	}

	inFile >> numIndices;
	indices = new GLushort[numIndices];
	for(int i = 0; i < numIndices; i++)
	{
		inFile >> indices[i];
	}

	inFile.close();
	return true;
}
