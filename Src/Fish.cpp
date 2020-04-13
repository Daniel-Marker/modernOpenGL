#include "Fish.h"

Fish::Fish(Shader* shader, Texture2D* texture, Mesh* mesh, Material* material, Camera* camera, Transform transform, RectCollider collisionRect):
	SceneObject(shader, texture, mesh, material, camera, transform, collisionRect), _time(0), _initPos(transform.position), _initRot(transform.rotation)
{
}

Fish::Fish(Shader* shader, Texture2D* texture, std::string meshPath, Material* material, Camera* camera, Transform transform, RectCollider collisionRect):
	SceneObject(shader, texture, meshPath, material, camera, transform, collisionRect), _time(0), _initPos(transform.position), _initRot(transform.rotation)
{
}

Fish::~Fish()
{
}

void Fish::Update(float deltaTime)
{
	_time += deltaTime;

	_transform.position.y = _initPos.y + glm::sin(_time);
	_transform.position.z = _initPos.z + cZAmplitude * glm::cos(_time);

	if (glm::cos(_time) > 0.0f)
		_transform.rotation.y = glm::lerp(_initRot.y, glm::radians(90.0f), glm::cos(_time));
	else
		_transform.rotation.y = glm::lerp(_initRot.y, glm::radians(270.0f), -glm::cos(_time));
}

void Fish::Render()
{
	_shader->SetUniformFloat(_time, "u_Time");
	_shader->SetUniformFloat(cWobbleFreq, "u_Freq");
	_shader->SetUniformFloat(cWobbleAmplitude, "u_Amplitude");
	_shader->SetUniformVec3(cAxisInput, "u_AxisInput");
	_shader->SetUniformVec3(cAxisEffect, "u_AxisEffect");

	SceneObject::Render();
}
