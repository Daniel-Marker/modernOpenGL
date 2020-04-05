#include "Camera.h"

glm::vec3 Camera::LookRotation(glm::vec2 rotation)
{
	glm::vec3 LookDirection;

	LookDirection.x = cos(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
	LookDirection.y = sin(glm::radians(rotation.y));
	LookDirection.z = sin(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
	LookDirection = glm::normalize(LookDirection);

	return LookDirection;
}

Camera::Camera(glm::vec3 position, glm::vec2 rotation, glm::vec3 up, RectCollider rectCollider):
	_position(position), _rotation(rotation), _up(up), _direction(LookRotation(rotation)), _rectCollider(rectCollider)
{
	
}

void Camera::Move(glm::vec3 movement, std::vector<SceneObject*>& sceneObjects)
{
	glm::mat4 cameraMatrix =
		glm::translate(glm::mat4(1.0f), _position + movement) *
		glm::eulerAngleXYZ(_rotation.y, _rotation.x, 0.0f);

	bool colliding = false;
	for (int i = 0; i < sceneObjects.size(); i++)
	{
		if (_rectCollider.CollisionCheck(cameraMatrix, sceneObjects[i]->GetRectCollider(), sceneObjects[i]->GetTransformMatrix()))
		{
			colliding = true;
			break;
		}
	}
	if (!colliding)
		_position += movement;
}

void Camera::Update(float deltaTime, std::vector<SceneObject*>& sceneObjects)
{
	HandleMouseInput(deltaTime);
	HandleMovement(deltaTime, sceneObjects);
}

void Camera::HandleMovement(float deltaTime, std::vector<SceneObject*>& sceneObjects)
{
	glm::vec3 cameraRight = glm::normalize(glm::cross(_direction, _up));
	if (InputManager::GetKeyDown('d'))
	{
		Move(cCameraMoveSpeed * deltaTime * cameraRight, sceneObjects);
	}

	if (InputManager::GetKeyDown('a'))
	{
		Move(-cCameraMoveSpeed * deltaTime * cameraRight, sceneObjects);
	}

	if (InputManager::GetKeyDown('w'))
	{
		Move(cCameraMoveSpeed * deltaTime * _direction, sceneObjects);
	}

	if (InputManager::GetKeyDown('s'))
	{
		Move(-cCameraMoveSpeed * deltaTime * _direction, sceneObjects);
	}
}

void Camera::HandleMouseInput(float deltaTime)
{
	int deltaX, deltaY;
	InputManager::GetMouseMovement(deltaX, deltaY);
	_rotation.x += cMouseSensitivity * deltaX * deltaTime;
	_rotation.y -= cMouseSensitivity * deltaY * deltaTime;

	if (_rotation.y > cCameraMax)
		_rotation.y = cCameraMax;
	if (_rotation.y < cCameraMin)
		_rotation.y = cCameraMin;

	_direction = LookRotation(_rotation);
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(_position, _position + _direction, _up);
}
