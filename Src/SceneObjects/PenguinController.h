#pragma once
#include "SceneObject.h"
class PenguinController :
	public SceneObject
{
public:
	PenguinController(Shader* shader, Texture2D* texture, Mesh* mesh, Material* material, Camera* camera, Transform transform, RectCollider collisionRect);
	PenguinController(Shader* shader, Texture2D* texture, std::string meshPath, Material* material, Camera* camera, Transform transform, RectCollider collisionRect);
	~PenguinController();
	void Update(float deltaTime);
};

