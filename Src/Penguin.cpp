#include "Penguin.h"

Penguin::Penguin(Shader* shader, Texture2D* texture, Mesh* mesh, Material* material, Camera* camera,
	Transform transform, RectCollider collisionRect, float timeOffset):
	SceneObject(shader, texture, mesh, material, camera, transform, collisionRect), _time(timeOffset), _initPos(transform.position),
	_period(4.0f), _amplitude(2.0f)
{
}

Penguin::Penguin(Shader* shader, Texture2D* texture, std::string meshPath, Material* material, Camera* camera,
	Transform transform, RectCollider collisionRect, float timeOffset):
	SceneObject(shader, texture, meshPath, material, camera, transform, collisionRect), _time(timeOffset), _initPos(transform.position),
	_period(4.0f), _amplitude(2.0f)
{
}

Penguin::~Penguin()
{
}

void Penguin::Update(float deltaTime)
{
	_time += deltaTime;

	_transform.position.z = _initPos.z + _amplitude * glm::sin(_time * 2.0f * glm::pi<float>() / _period);
}