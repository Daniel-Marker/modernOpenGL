#pragma once
#include "SceneObject.h"
class Fish :
	public SceneObject
{
private:
	float _time;
	glm::vec3 _initPos;
	glm::vec3 _initRot;

public:
	Fish(Shader* shader, Texture2D* texture, Mesh* mesh, Material* material, Camera* camera, Transform transform, RectCollider collisionRect);
	Fish(Shader* shader, Texture2D* texture, std::string meshPath, Material* material, Camera* camera, Transform transform, RectCollider collisionRect);
	~Fish();
	void Update(float deltaTime);
};

