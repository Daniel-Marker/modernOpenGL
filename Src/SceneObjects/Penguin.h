#pragma once
#include "SceneObject.h"
class Penguin :
	public SceneObject
{
private:
	float _time;
	float _timeHeld;

	const glm::vec3 cInitPos;
	const glm::vec3 cInitRot;
	const float cPeriod;
	const float cAmplitude;
	const float cTimeHeldMultiplier;
	const float cMaxRotation;

public:
	Penguin(Shader* shader, Texture2D* texture, Mesh* mesh, Material* material, Camera* camera, Transform transform, RectCollider collisionRect, float timeOffset);
	Penguin(Shader* shader, Texture2D* texture, std::string meshPath, Material* material, Camera* camera, Transform transform, RectCollider collisionRect, float timeOffset);
	~Penguin();
	void Update(float deltaTime);
};

