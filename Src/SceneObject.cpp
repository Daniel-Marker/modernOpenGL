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
	for (int i = 0; i < _children.size(); i++)
		delete _children[i];

	_children.clear();
}

void SceneObject::Render(glm::mat4& worldTransform)
{
	for (int i = 0; i < _children.size(); i++)
	{
		_children[i]->Render(worldTransform);
	}
}

void SceneObject::Update(float deltaTime)
{
	for (int i = 0; i < _children.size(); i++)
	{
		_children[i]->Update(deltaTime);
	}
}

const Transform SceneObject::GetTransform()
{
	return _transform;
}

void SceneObject::SetTransform(Transform transform)
{
	_transform = transform;
}

void SceneObject::AddChild(SceneObject* child)
{
	_children.push_back(child);
}

std::vector<SceneObject*> const SceneObject::GetChildren()
{
	return _children;
}

bool SceneObject::operator<(const SceneObject& other) const
{
	float distToCamera = glm::distance(_transform.position, _camera->center);
	float distFromOtherToCamera = glm::distance(other._transform.position, _camera->center);

	return distToCamera < distFromOtherToCamera;
}
