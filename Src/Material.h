#pragma once

#include "glm/common.hpp"
#include "glm/mat4x4.hpp"
#include "Shader.h"

class Material
{
private:
	glm::vec3 _ambient;
	glm::vec3 _diffuse;
	glm::vec3 _specular;
	float _shininess;

public:
	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);

	void SetMaterialUniforms(Shader* shader);
};

