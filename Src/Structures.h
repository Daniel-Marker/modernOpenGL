#pragma once

#include "glm/glm.hpp"

struct Camera {
	glm::vec3 position, direction, up;
	glm::vec2 rotation;
};

struct Transform 
{
	glm::vec3 position, rotation, scale;
};