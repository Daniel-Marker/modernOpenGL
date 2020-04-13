#pragma once
#include "SceneObject.h"
class Penguin :
	public SceneObject
{
private:
	float _time;

	const glm::vec3 _initPos;
	const float _period;
	const float _amplitude;

public:
	Penguin(Shader* shader, Texture2D* texture, Mesh* mesh, Material* material, Camera* camera, Transform transform, RectCollider collisionRect, float timeOffset);
	Penguin(Shader* shader, Texture2D* texture, std::string meshPath, Material* material, Camera* camera, Transform transform, RectCollider collisionRect, float timeOffset);
	~Penguin();
	void Update(float deltaTime);
};

