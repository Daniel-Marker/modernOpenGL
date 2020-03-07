#include "SceneObject.h"

SceneObject::SceneObject(Shader* shader, InputManager* inputManager, Texture2D* texture, Mesh* mesh, Material* material):
	_shader(shader), _inputManager(inputManager), _texture(texture), _mesh(mesh), _material(material)
{
	_transform.position = glm::vec3(0.0f);
	_transform.rotation = glm::vec3(0.0f);
	_transform.scale = glm::vec3(0.0f);
}

SceneObject::~SceneObject()
{
}

void SceneObject::Render()
{
}

void SceneObject::Update(float deltaTime)
{
}

const Transform SceneObject::GetTransform()
{
	return _transform;
}

void SceneObject::SetTransform(Transform transform)
{
	_transform = transform;
}
