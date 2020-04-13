#include "PenguinController.h"

PenguinController::PenguinController(Shader* shader, Texture2D* texture, Mesh* mesh, Material* material, Camera* camera, Transform transform, RectCollider collisionRect):
	SceneObject(shader, texture, mesh, material, camera, transform, collisionRect)
{
}

PenguinController::PenguinController(Shader* shader, Texture2D* texture, std::string meshPath, Material* material, Camera* camera, Transform transform, RectCollider collisionRect):
	SceneObject(shader, texture, meshPath, material, camera, transform, collisionRect)
{
}

PenguinController::~PenguinController()
{
}

void PenguinController::Update(float deltaTime)
{
	_transform.rotation.y += deltaTime * -1.0f;

	SceneObject::Update(deltaTime);
}
