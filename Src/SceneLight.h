#pragma once

#include "Structures.h"
#include "glm/common.hpp"

class SceneLight
{
private:
	glm::vec3 _color;
	float _intensity;
	glm::vec3 _position;

public:
	SceneLight(glm::vec3 color, float intensity, glm::vec3 position);
	~SceneLight();

	const float GetIntensity() { return _intensity; };
	void SetIntensity(float intensity) { _intensity = intensity; };

	const glm::vec3 GetColor() { return _color; };
	void SetColor(glm::vec3 color) { _color = color; };

	const glm::vec3 GetPosition() { return _position; };
	void SetPosition(glm::vec3 position) { _position = position; };
};

