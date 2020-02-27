#pragma once

#include "Structures.h"
#include "InputManager.h"

#include "Shader.h"
#include "Texture2D.h"
#include "Vao.h"

class SceneObject
{
protected:
	Shader* _shader;
	Vao* _vao;

	Texture2D* _texture;

	InputManager* _inputManager;

	Transform _transform;

public:
	SceneObject(Shader* shader, InputManager* inputManager, Texture2D* texture);
	virtual ~SceneObject();
	virtual void Render();
	virtual void Update(float deltaTime);
	const Transform GetTransform();
	void SetTransform(Transform transform);
};

