#include "Room.h"

Room::Room(Shader* shader, Texture2D* texture, Mesh* mesh, Material* material, Camera* camera, Transform transform, RectCollider collisionRect, std::string collisionString, std::string* roomString):
	SceneObject(shader, texture, mesh, material, camera, transform, collisionRect), _collisionString(collisionString), _roomString(roomString)
{
}

Room::Room(Shader* shader, Texture2D* texture, std::string meshPath, Material* material, Camera* camera, Transform transform, RectCollider collisionRect, std::string collisionString, std::string* roomString):
	SceneObject(shader, texture, meshPath, material, camera, transform, collisionRect), _collisionString(collisionString), _roomString(roomString)
{
}

void Room::OnTrigger()
{
	*_roomString = _collisionString;
}
