#pragma once
#include <Windows.h>	//Required for OpenGL on windows
//#include <gl/GL.h>		//OpenGL
//#include <gl/GLU.h>		//OpenGL Utilities
#include "GL/glew.h"
#include "GL\freeglut.h"	//freeglut library

#include "GLUTCallbacks.h"
#include "InputManager.h"
#include "Structures.h"
#include "SceneObject.h"
#include "Shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
//#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/euler_angles.hpp"

class HelloGL
{
public:
	HelloGL(int argc, char* argv[]);
	~HelloGL();
	void Display();
	void Update(float deltaTime);

	InputManager* inputManager;


private:
	const int _cRefreshRate;

	Camera* camera;

	SceneObject* pyramids[200];
	Shader* shader;

	glm::mat4 viewProjMatrix;

};

