#pragma once

#include "Structures.h"
#include "glm/common.hpp"

class SceneLight
{
private:
	glm::vec4 _position;

	glm::vec3 _DiffuseColor;
	float _DiffuseIntensity;

	glm::vec3 _AmbientColor;
	float _AmbientIntensity;

	glm::vec3 _SpecularColor;
	float _SpecularIntensity;

public:
	SceneLight(glm::vec4 position, glm::vec3 DiffuseColor, float DiffuseIntensity, glm::vec3 AmbientColor, float AmbientIntensity, glm::vec3 SpecularColor, float SpecularIntensity);
	~SceneLight();

	glm::vec4& GetPosition() { return _position; };
	void SetPosition(glm::vec4 position) { _position = position; };

	glm::vec3& GetDiffuseColor() { return _DiffuseColor; };
	void SetDiffuseColor(glm::vec3 color) { _DiffuseColor = color; };
	float& GetDiffuseIntensity() { return _DiffuseIntensity; };
	void SetDiffuseIntensity(float intensity) { _DiffuseIntensity = intensity; };

	glm::vec3& GetAmbientColor() { return _AmbientColor; };
	void SetAmbientColor(glm::vec3 color) { _AmbientColor = color; };
	float& GetAmbientIntensity() { return _AmbientIntensity; };
	void SetAmbientIntensity(float intensity) { _AmbientIntensity = intensity; };

	glm::vec3& GetSpecularColor() { return _SpecularColor; };
	void SetSpecularColor(glm::vec3 color) { _SpecularColor = color; };
	float& GetSpecularIntensity() { return _SpecularIntensity; };
	void SetSpecularIntensity(float intensity) { _SpecularIntensity = intensity; };
};

