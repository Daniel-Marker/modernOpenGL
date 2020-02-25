#pragma once
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "GL\freeglut.h"
#include "Structures.h"
#include <fstream>
#include <iostream>

class Cube
{
private:
	static Vertex* indexedVertices;
	static Color* indexedColors;
	static GLushort* indices;

	static int numVertices, numColors, numIndices;

	Rotation _rotation;
	Vector3 _position;
	float _rotationSpeed;

public:
	Cube(float xPos, float yPos, float zPos, float xAngle, float yAngle, float zAngle, float rotSpeed);
	~Cube();

	void Draw();
	void Update();

	static bool Load(char* path);
};

