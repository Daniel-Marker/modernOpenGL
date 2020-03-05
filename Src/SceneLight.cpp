#include "SceneLight.h"
#include "SceneLight.h"

SceneLight::SceneLight(glm::vec4 position, glm::vec3 DiffuseColor, float DiffuseIntensity, glm::vec3 AmbientColor, float AmbientIntensity, glm::vec3 SpecularColor, float SpecularIntensity):
	_position(position), _DiffuseColor(DiffuseColor), _DiffuseIntensity(DiffuseIntensity),
	_AmbientColor(AmbientColor), _AmbientIntensity(AmbientIntensity),
	_SpecularColor(SpecularColor), _SpecularIntensity(SpecularIntensity)
{
}

SceneLight::~SceneLight()
{
}
