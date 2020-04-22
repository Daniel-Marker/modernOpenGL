#pragma once

#include <vector>

#include "SceneObject.h"
#include "InputManager.h"
#include "Constants.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"

class Camera
{
private:
	glm::vec3 _position, _direction, _up;
	glm::vec2 _rotation;
	RectCollider _rectCollider;

	glm::vec3 LookRotation(glm::vec2 rotation);

public:
	Camera(glm::vec3 position, glm::vec2 rotation, glm::vec3 up, RectCollider rectCollider);
	void Move(glm::vec3 movement, std::vector<SceneObject*>& sceneObjects);
	void Update(float deltaTime, std::vector<SceneObject*>& sceneObjects);

	void HandleMovement(float deltaTime, std::vector<SceneObject*>& sceneObjects);
	void HandleMouseInput(float deltaTime);
	void HandleTriggers(std::vector<SceneObject*>& sceneObjects);

	glm::mat4 GetViewMatrix();
	glm::vec3 GetPosition() { return _position; };
	glm::vec3 GetDirection() { return _direction; };
};

