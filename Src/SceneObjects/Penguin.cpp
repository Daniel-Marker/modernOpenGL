#include "Penguin.h"

Penguin::Penguin(Shader* shader, Texture2D* texture, Mesh* mesh, Material* material, Camera* camera,
	Transform transform, RectCollider collisionRect, float timeOffset):
	SceneObject(shader, texture, mesh, material, camera, transform, collisionRect), _time(timeOffset), _cInitPos(transform.position), _cInitRot(transform.rotation),
	_cPeriod(4.0f), _cAmplitude(2.0f), _timeHeld(0.0f), _cTimeHeldMultiplier(5.0f)
{
}

Penguin::Penguin(Shader* shader, Texture2D* texture, std::string meshPath, Material* material, Camera* camera,
	Transform transform, RectCollider collisionRect, float timeOffset):
	SceneObject(shader, texture, meshPath, material, camera, transform, collisionRect), _time(timeOffset), _cInitPos(transform.position), _cInitRot(transform.rotation),
	_cPeriod(4.0f), _cAmplitude(2.0f), _timeHeld(0.0f), _cTimeHeldMultiplier(5.0f)
{
}

Penguin::~Penguin()
{
}

void Penguin::Update(float deltaTime)
{
	_time += deltaTime;

	_transform.position.z = _cInitPos.z + _cAmplitude * glm::sin(_time * 2.0f * glm::pi<float>() / _cPeriod);

	if (InputManager::GetKeyDown('p'))
	{
		_timeHeld += deltaTime;
		_transform.rotation.z = glm::radians(15.0f * glm::sin(_timeHeld * _cTimeHeldMultiplier));
	}
	else
	{
		_timeHeld = 0.0f;
		_transform.rotation.z = _cInitRot.z;
	}
}