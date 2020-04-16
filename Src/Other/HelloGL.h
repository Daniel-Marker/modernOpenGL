#pragma once
#include "GL/glew.h"
#include "GL\freeglut.h"	//freeglut library

#include "GLUTCallbacks.h"
#include "InputManager.h"
#include "Structures.h"
#include "Shader.h"
#include "Constants.h"
#include "Texture2D.h"
#include "Mesh.h"
#include "SceneLight.h"
#include "Material.h"
#include "Font.h"
#include "RectCollider.h"
#include "Camera.h"
#include "Skybox.h"
#include "Fish.h"
#include "PenguinController.h"
#include "Penguin.h"

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
	Shader* skyboxShader;
	Shader* fishShader;

	Mesh* cubeMesh;
	Mesh* emptyMesh;
	Mesh* entranceWall1;
	Mesh* entranceWall2;
	Mesh* entranceWall3;
	Mesh* outsideFence;
	Mesh* mainWall;
	Mesh* sideWall;
	Mesh* penguin;


	std::vector<SceneLight*> sceneLights;
	bool lightsDim;

	Material* basicMaterial;
	Material* woodMaterial;

	Texture2D* mapTexture;
	Texture2D* grassTexture;
	Texture2D* fenceTexture;
	Texture2D* fishTankTexture;
	Texture2D* fishTexture;
	Texture2D* penguinTexture;
	Texture2D* fossilTexture;
	Texture2D* blankTexture;

	Font* font;

	Skybox* skybox;

	glm::mat4 viewProjMatrix;

	int windowWidth, windowHeight;
	float aspectRatio;
};

