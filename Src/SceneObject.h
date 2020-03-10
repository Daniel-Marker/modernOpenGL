#pragma once

#include <vector>

#include "Structures.h"
#include "InputManager.h"

#include "Shader.h"
#include "Texture2D.h"
#include "Vao.h"
#include "Mesh.h"
#include "Material.h"

class SceneObject
{
protected:
	Transform _transform;
	Mesh* _mesh;
	
	Vao* _vao;
	Shader* _shader;
	Texture2D* _texture;
	Material* _material;

	Camera* _camera;
	bool _isTransparent;

	std::vector<SceneObject*> _children;

public:
	SceneObject(Shader* shader, Texture2D* texture, Mesh* mesh, Material* material, Camera* camera);
	virtual ~SceneObject();
	virtual void Render(glm::mat4& worldTransform);
	virtual void Update(float deltaTime);
	const Transform GetTransform();
	void SetTransform(Transform transform);
	bool GetTransparent() { return _isTransparent; };
	void AddChild(SceneObject* child);
	std::vector<SceneObject*> const GetChildren();

	bool operator< (const SceneObject& other) const;
};

