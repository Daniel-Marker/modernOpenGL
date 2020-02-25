#pragma once

#include "GL/glew.h"
#include "GL\freeglut.h"

#include "Structures.h"
#include "InputManager.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"

class SceneObject
{
private:
	GLuint shader;
	GLuint vao;
	
	InputManager* inputManager;

	static float positions[];
	static unsigned int indices[];

public:
	SceneObject(GLuint shader, InputManager* inputManagerInput);
	~SceneObject();
	void Render(glm::mat4 viewProjMatrix);
	void Update();

public:
	Transform transform;
};

