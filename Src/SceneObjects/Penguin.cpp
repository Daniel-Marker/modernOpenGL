#include "Penguin.h"

Penguin::Penguin(Shader* shader, Texture2D* texture, Mesh* mesh, Material* material, Camera* camera,
	Transform transform, RectCollider collisionRect, float timeOffset):
	SceneObject(shader, texture, mesh, material, camera, transform, collisionRect), _time(timeOffset), cInitPos(transform.position), cInitRot(transform.rotation),
	cPeriod(4.0f), cAmplitude(2.0f), _timeHeld(0.0f), cTimeHeldMultiplier(5.0f), cMaxRotation(15.0f)
{
}

Penguin::Penguin(Shader* shader, Texture2D* texture, std::string meshPath, Material* material, Camera* camera,
	Transform transform, RectCollider collisionRect, float timeOffset):
	SceneObject(shader, texture, meshPath, material, camera, transform, collisionRect), _time(timeOffset), cInitPos(transform.position), cInitRot(transform.rotation),
	cPeriod(4.0f), cAmplitude(2.0f), _timeHeld(0.0f), cTimeHeldMultiplier(5.0f), cMaxRotation(15.0f)
{
}

Penguin::~Penguin()
{
}

void Penguin::Update(float deltaTime)
{
	_time += deltaTime;

	_transform.position.z = cInitPos.z + cAmplitude * glm::sin(_time * 2.0f * glm::pi<float>() / cPeriod);

	if (InputManager::GetKeyDown('p'))
	{
		_timeHeld += deltaTime;
		_transform.rotation.z = glm::radians(cMaxRotation * glm::sin(_timeHeld * cTimeHeldMultiplier));
	}
	else
	{
		_timeHeld = 0.0f;
		_transform.rotation.z = cInitRot.z;
	}
}