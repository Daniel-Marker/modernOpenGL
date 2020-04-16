#pragma once
#include "SceneObject.h"
class Penguin :
	public SceneObject
{
private:
	float _time;
	float _timeHeld;

	const glm::vec3 _cInitPos;
	const glm::vec3 _cInitRot;
	const float _cPeriod;
	const float _cAmplitude;
	const float _cTimeHeldMultiplier;

public:
	Penguin(Shader* shader, Texture2D* texture, Mesh* mesh, Material* material, Camera* camera, Transform transform, RectCollider collisionRect, float timeOffset);
	Penguin(Shader* shader, Texture2D* texture, std::string meshPath, Material* material, Camera* camera, Transform transform, RectCollider collisionRect, float timeOffset);
	~Penguin();
	void Update(float deltaTime);
};

