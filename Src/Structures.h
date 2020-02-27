#pragma once

#include "glm/glm.hpp"

struct Camera {
	glm::vec3 eye, center, up;
};

struct Transform 
{
	glm::vec3 position, rotation, scale;
};