#pragma once

#include "glm/glm.hpp"

struct Camera {
	glm::vec3 position, direction, up;
	glm::vec2 rotation;
};

struct Transform 
{
	glm::vec3 position, rotation, scale;

	Transform(glm::vec3 inputPosition, glm::vec3 inputRotation, glm::vec3 inputScale) :
		position(inputPosition), rotation(inputRotation), scale(inputScale)
	{};

	Transform():
		position(glm::vec3(0.0f)), rotation(glm::vec3(0.0f)), scale(glm::vec3(0.0f))
	{}
};