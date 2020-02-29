#pragma once
#include <Windows.h>	//Required for OpenGL on windows
//#include <gl/GL.h>		//OpenGL
//#include <gl/GLU.h>		//OpenGL Utilities
#include "GL/glew.h"
#include "GL\freeglut.h"	//freeglut library

#include "GLUTCallbacks.h"
#include "InputManager.h"
#include "Structures.h"
#include "Cube.h"
#include "Shader.h"
#include "Constants.h"
#include "Texture2D.h"

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
	void OnResize(int height, int width);
	void InitGL(int argc, char* argv[]);
	void InitObjects();
	void LoadTextures();
	void InitShaders();

	InputManager* inputManager;


private:
	Camera* camera;

	SceneObject* sceneObjects[200];
	Shader* shader;

	Texture2D* penguinTexture;
	Texture2D* parrotTexture;
	Texture2D* parrotTexture32;
	Texture2D* parrotTextureTGA;

	glm::mat4 viewProjMatrix;

	int windowWidth, windowHeight;
	float aspectRatio;
};

