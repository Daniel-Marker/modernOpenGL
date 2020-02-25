#pragma once

#include "glm/glm.hpp"

struct Vector3 {
	float x, y, z;
};

struct Camera {
	Vector3 eye, center, up;
};

struct Color
{
	GLfloat r, g, b;
};

struct Vertex
{
	GLfloat x, y, z;
};

struct Rotation
{
	float x, y, z;

	Rotation() 
	{
		x = 0.0f; y = 0.0f; z = 0.0f;
	}

	Rotation(float xAngle, float yAngle, float zAngle)
	{
		x = xAngle; y = yAngle; z = zAngle;
	}
};

struct Transform 
{
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};