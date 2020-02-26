#pragma once

#include "GL/glew.h"
#include "GL\freeglut.h"

#include "Structures.h"
#include "InputManager.h"

#include "Shader.h"
#include "Vao.h"
#include "Renderer.h"
#include "Texture2D.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"

class SceneObject
{
private:
	Shader* _shader;
	Vao* _vao;
	
	Texture2D* _texture;

	InputManager* _inputManager;

	static float vertexData[];
	static unsigned int indices[];

public:
	SceneObject(Shader* shader, InputManager* inputManager, Texture2D* texture);
	~SceneObject();
	void Render();
	void Update(float deltaTime);

public:
	Transform transform;
};

