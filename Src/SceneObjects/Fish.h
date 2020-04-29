#pragma once
#include "SceneObject.h"
class Fish :
	public SceneObject
{
private:
	float _time;

	const glm::vec3 cInitPos;
	const glm::vec3 cInitRot;
	const float cZAmplitude = 4.0f;
	const float cWobbleFreq = 48.8224258f;
	const float cWobbleAmplitude = 0.0440003984f;
	const glm::vec3 cAxisInput = glm::vec3(1.0f, 0.0f, 0.0f);
	const glm::vec3 cAxisEffect = glm::vec3(0.0f, 0.5f, 1.0f);

public:
	Fish(Shader* shader, Texture2D* texture, Mesh* mesh, Material* material, Camera* camera, Transform transform, RectCollider collisionRect);
	Fish(Shader* shader, Texture2D* texture, std::string meshPath, Material* material, Camera* camera, Transform transform, RectCollider collisionRect);
	~Fish();
	void Update(float deltaTime);
	void Render();
};

