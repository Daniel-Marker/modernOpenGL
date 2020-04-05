#pragma once
#include "GL/glew.h"
#include "GL\freeglut.h"	//freeglut library

#include "GLUTCallbacks.h"
#include "InputManager.h"
#include "Structures.h"
#include "Cube.h"
#include "Shader.h"
#include "Constants.h"
#include "Texture2D.h"
#include "Mesh.h"
#include "SceneLight.h"
#include "Material.h"
#include "Font.h"
#include "RectCollider.h"
#include "Camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"

#include <vector>
#include <algorithm>

class HelloGL
{
public:
	HelloGL(int argc, char* argv[]);
	~HelloGL();
	void Display();
	void Update(float deltaTime);
	void OnResize(int height, int width);
	void InitGL(int argc, char* argv[]);
	void InitLights();
	void InitObjects();
	void LoadTextures();
	void LoadMeshes();
	void InitMaterials();
	void InitShaders();

	int GetWidth() { return windowWidth; };
	int GetHeight() { return windowHeight; };

private:
	Camera* camera;
	std::vector<SceneObject*> sceneObjects;

	Shader* lightingShader;
	Shader* basicShader;
	Shader* textShader;

	Mesh* cubeMesh;
	Mesh* betterCubeMesh;
	std::vector<SceneLight*> sceneLights;
	Material* basicMaterial;

	Texture2D* penguinTexture;
	Texture2D* parrotTexture;
	Texture2D* parrotTexture32;
	Texture2D* parrotTextureTGA;
	Texture2D* betterCubeTexture;
	Texture2D* glassTexture;
	Texture2D* glassTexture2;
	Texture2D* glassTexture3;

	Font* font;

	glm::mat4 viewProjMatrix;

	int windowWidth, windowHeight;
	float aspectRatio;

	float textRotation;
};

