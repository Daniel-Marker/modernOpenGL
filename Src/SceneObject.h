#pragma once

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
	InputManager* _inputManager;

	Transform _transform;
	Mesh* _mesh;
	
	Vao* _vao;
	Shader* _shader;
	Texture2D* _texture;
	Material* _material;

public:
	SceneObject(Shader* shader, InputManager* inputManager, Texture2D* texture, Mesh* mesh, Material* material);
	virtual ~SceneObject();
	virtual void Render();
	virtual void Update(float deltaTime);
	const Transform GetTransform();
	void SetTransform(Transform transform);
};

