#pragma once

#include "GL/glew.h"
#include "GL\freeglut.h"

#include "Structures.h"
#include "InputManager.h"

#include "Shader.h"
#include "Vao.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"

class SceneObject
{
private:
	Shader* shader;
	Vao* vao;
	
	InputManager* inputManager;

	static float positions[];
	static unsigned int indices[];

public:
	SceneObject(Shader* shader, InputManager* inputManagerInput);
	~SceneObject();
	void Render(glm::mat4 viewProjMatrix);
	void Update();

public:
	Transform transform;
};

