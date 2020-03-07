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

void SceneLight::SetLightUniforms(glm::vec3 cameraPos, std::string lightNo, Shader* shader)
{
	shader->SetUniformVec4(_position, lightNo + ".LightPos");
	shader->SetUniformVec3(cameraPos, lightNo + ".CameraPos");
	shader->SetUniformVec3(_DiffuseColor, lightNo + ".DiffuseColor");
	shader->SetUniformFloat(_DiffuseIntensity, lightNo + ".DiffuseIntensity");
	shader->SetUniformVec3(_AmbientColor, lightNo + ".AmbientColor");
	shader->SetUniformFloat(_AmbientIntensity, lightNo + ".AmbientIntensity");
	shader->SetUniformVec3(_SpecularColor, lightNo + ".SpecularColor");
	shader->SetUniformFloat(_SpecularIntensity, lightNo + ".SpecularIntensity");
}
