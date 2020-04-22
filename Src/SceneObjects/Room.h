#pragma once
#include "SceneObject.h"
class Room :
	public SceneObject
{
private:
	std::string _collisionString;
	std::string* _roomString;

public:
	Room(Shader* shader, Texture2D* texture, Mesh* mesh, Material* material, Camera* camera, Transform transform, RectCollider collisionRect, std::string collisionString, std::string* roomString);
	Room(Shader* shader, Texture2D* texture, std::string meshPath, Material* material, Camera* camera, Transform transform, RectCollider collisionRect, std::string collisionString, std::string* roomString);

	void OnTrigger();
};

