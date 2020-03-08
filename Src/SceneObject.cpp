#include "SceneObject.h"

SceneObject::SceneObject(Shader* shader, Texture2D* texture, Mesh* mesh, Material* material, Camera* camera):
	_shader(shader), _texture(texture), _mesh(mesh), _material(material), _camera(camera)
{
	_transform.position = glm::vec3(0.0f);
	_transform.rotation = glm::vec3(0.0f);
	_transform.scale = glm::vec3(0.0f);

	_isTransparent = texture->GetTransparent();
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

bool SceneObject::operator<(const SceneObject& other) const
{
	float distToCamera = glm::distance(_transform.position, _camera->center);
	float distFromOtherToCamera = glm::distance(other._transform.position, _camera->center);

	return distToCamera < distFromOtherToCamera;
}
