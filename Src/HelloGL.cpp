#include "HelloGL.h"

#include <iostream>
#include <string>

//todo
//Update texture code to take a string instead of char as an input
//Update mesh to use a vector instead of dynamic array

//Todo For lighting:
//Make material class
//Update shaders to handle materials
//Figure out a way to have multiple light sources be inputs to the shader (maybe an array of the nearest lights?)
//Set the light position to be a vec4 and have the w component specify if the light is a point light or directional (0 = direction, 1 = point)
//trying setting diffuse, ambient and specular lighting input to fragment shader as varying

//todo after up to date with tutorials:
//Have code actually use the return value of texture load
//fix repeated vertices problem, so that _vertexCount = tempVertexCount * 3, instead of _vertexCount = tempFaceCount * 3;
//auto triangulate faces in obj loader if they consist of more than 3 vertices
//Have obj loader be able to handle files with multiple objects
//add special keys callback
//maybe make input manager a static object
//Figure out if worth using a linked list for list/array of scene objects
//Sort rendering order for transparent objects

HelloGL::HelloGL(int argc, char* argv[])
{
	InitGL(argc, argv);
	InitShaders();

	inputManager = new InputManager();

	LoadTextures();

	camera = new Camera;
	camera->eye = glm::vec3(0.0f, 0.0f, -5.0f);
	camera->center = glm::vec3(0.0f, 0.0f, 0.0f);
	camera->up = glm::vec3(0.0f, 1.0f, 0.0f);

	InitObjects();
	glutMainLoop();
}


HelloGL::~HelloGL()
{
	delete camera;
	delete inputManager;
	delete lightingShader;

	delete penguinTexture;
	delete parrotTexture;
	delete parrotTexture32;
	delete parrotTextureTGA;
	delete betterCubeTexture;

	for (int i = 0; i < 200; i++)
	{
		delete sceneObjects[i];
	}

	delete cubeMesh;
	delete betterCubeMesh;
}

void HelloGL::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*viewProjMatrix = glm::perspective(cFOV, aspectRatio, cNearClippingPlaneDist, cFarClippingPlaneDist) *
		glm::lookAt(camera->eye,camera->center, camera->up);
	shader->SetUniformMatrix(viewProjMatrix, "u_VP");*/

	glm::mat4 viewMatrix = glm::lookAt(camera->eye, camera->center, camera->up);
	glm::mat4 projMatrix = glm::perspective(cFOV, aspectRatio, cNearClippingPlaneDist, cFarClippingPlaneDist);
	lightingShader->SetUniformMatrix(viewMatrix, "u_View");
	lightingShader->SetUniformMatrix(projMatrix, "u_Proj");

	basicShader->SetUniformMatrix(viewMatrix, "u_View");
	basicShader->SetUniformMatrix(projMatrix, "u_Proj");

	lightingShader->SetUniformVec3(light->GetPosition(), "u_LightPos");
	lightingShader->SetUniformVec3(camera->center, "u_CameraPos");
	lightingShader->SetUniformVec3(light->GetDiffuseColor(), "u_DiffuseColor");
	lightingShader->SetUniformFloat(light->GetDiffuseIntensity(), "u_DiffuseIntensity");
	lightingShader->SetUniformVec3(light->GetAmbientColor(), "u_AmbientColor");
	lightingShader->SetUniformFloat(light->GetAmbientIntensity(), "u_AmbientIntensity");
	lightingShader->SetUniformVec3(light->GetSpecularColor(), "u_SpecularColor");
	lightingShader->SetUniformFloat(light->GetSpecularIntensity(), "u_SpecularIntensity");

	for(int i = 0; i < 200; i++)
		sceneObjects[i]->Render();

	glFlush();
	glutSwapBuffers();
}

void HelloGL::Update(float deltaTime)
{
	for(int i = 0; i < 200; i++)
		sceneObjects[i]->Update(deltaTime);

	if (inputManager->GetKeyDown('8'))
	{
		camera->eye.z += 5.0f * deltaTime;
	}
	else if (inputManager->GetKeyDown('2'))
	{
		camera->eye.z -= 5.0f * deltaTime;
	}

	if (inputManager->GetKeyDown('d'))
	{
		camera->center.x -= 5.0f * deltaTime;
		camera->eye.x -= 5.0f * deltaTime;
	}

	if (inputManager->GetKeyDown('a'))
	{
		camera->center.x += 5.0f * deltaTime;
		camera->eye.x += 5.0f * deltaTime;
	}

	if (inputManager->GetKeyDown('w'))
	{
		camera->center.y += 5.0f * deltaTime;
		camera->eye.y += 5.0f * deltaTime;
	}

	if (inputManager->GetKeyDown('s'))
	{
		camera->center.y -= 5.0f * deltaTime;
		camera->eye.y -= 5.0f * deltaTime;
	}


	if (inputManager->GetKeyDown('i'))
	{
		light->SetPosition(light->GetPosition() + glm::vec3(0.0f, 5.0f, 0.0f) * deltaTime);
	}
	if (inputManager->GetKeyDown('j'))
	{
		light->SetPosition(light->GetPosition() + glm::vec3(5.0f, 0.0f, 0.0f) * deltaTime);
	}
	if (inputManager->GetKeyDown('k'))
	{
		light->SetPosition(light->GetPosition() + glm::vec3(0.0f, -5.0f, 0.0f) * deltaTime);
	}
	if (inputManager->GetKeyDown('l'))
	{
		light->SetPosition(light->GetPosition() + glm::vec3(-5.0f, 0.0f, 0.0f) * deltaTime);
	}
	if (inputManager->GetKeyDown('o'))
	{
		light->SetDiffuseIntensity(light->GetDiffuseIntensity() + 1.0f * deltaTime);
	}
	if (inputManager->GetKeyDown('p'))
	{
		float newIntensity = light->GetDiffuseIntensity() - 1.0f * deltaTime;
		if (newIntensity < 0.0f)
			newIntensity = 0.0f;

		light->SetDiffuseIntensity(newIntensity);
	}

	glutPostRedisplay();
}

void HelloGL::OnResize(int height, int width)
{
	windowHeight = height;
	windowWidth = width;
	glViewport(0, 0, windowWidth, windowHeight);
	aspectRatio = ((float)((float)windowWidth / (float)(windowHeight)));
}

void HelloGL::InitGL(int argc, char* argv[])
{
	GLUTCallbacks::Init(this);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(cInitWindowWidth, cInitWindowHeight);
	glutInitWindowPosition(cInitWindowPositionX, cInitWindowPositionY);
	glutCreateWindow("Simple OpenGL Program");

	windowHeight = cInitWindowHeight;
	windowWidth = cInitWindowWidth;
	glViewport(0, 0, windowWidth, windowHeight);
	aspectRatio = ((float)((float)windowWidth / (float)(windowHeight)));

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	std::cout << "Using OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	glutDisplayFunc(GLUTCallbacks::Display);
	glutTimerFunc(cRefreshRate, GLUTCallbacks::Timer, cRefreshRate);
	glutKeyboardFunc(GLUTCallbacks::Keyboard);
	glutKeyboardUpFunc(GLUTCallbacks::KeyboardUp);
	glutReshapeFunc(GLUTCallbacks::WindowResize);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glEnable(GL_ALPHA);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void HelloGL::InitObjects()
{
	glm::vec3 position = glm::vec3(0.0f, 0.0f, -5.0f);
	glm::vec3 diffuseColor = glm::vec3(0.0f, 0.0f, 1.0f);
	float diffuseIntensity = 0.6f;
	glm::vec3 ambientColor = glm::vec3(1.0f, 0.0f, 0.0f);
	float ambientIntensity = 0.1f;
	glm::vec3 specularColor = glm::vec3(0.0f, 1.0f, 0.0f);
	float specularIntensity = 1.0f;

	light = new SceneLight(position, diffuseColor, diffuseIntensity, ambientColor, ambientIntensity, specularColor, specularIntensity);

	cubeMesh = new Mesh("Res/Models/Cube.obj");
	betterCubeMesh = new Mesh("Res/Models/BetterCube.obj");

	for (int i = 0; i < 50; i++)
	{
		sceneObjects[i] = new Cube(lightingShader, inputManager, betterCubeTexture, betterCubeMesh);

		Transform transform;
		transform.position = glm::vec3((rand() % 200) / 10.0f, (rand() % 200) / 10.0f, (rand() % 200) / 10.0f);
		transform.rotation = glm::vec3(rand() % 360, rand() % 360, rand() % 360);
		transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);

		sceneObjects[i]->SetTransform(transform);
	}
	
	for (int i = 50; i < 100; i++)
	{
		sceneObjects[i] = new Cube(lightingShader, inputManager, parrotTexture, cubeMesh);

		Transform transform;
		transform.position = glm::vec3((rand() % 200) / 10.0f, (rand() % 200) / 10.0f, (rand() % 200) / 10.0f);
		transform.rotation = glm::vec3(rand() % 360, rand() % 360, rand() % 360);
		transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);

		sceneObjects[i]->SetTransform(transform);
	}

	for (int i = 100; i < 150; i++)
	{
		sceneObjects[i] = new Cube(lightingShader, inputManager, parrotTexture32, cubeMesh);

		Transform transform;
		transform.position = glm::vec3((rand() % 200) / 10.0f, (rand() % 200) / 10.0f, (rand() % 200) / 10.0f);
		transform.rotation = glm::vec3(rand() % 360, rand() % 360, rand() % 360);
		transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);

		sceneObjects[i]->SetTransform(transform);
	}

	for (int i = 150; i < 200; i++)
	{
		sceneObjects[i] = new Cube(basicShader, inputManager, penguinTexture, cubeMesh);

		Transform transform;
		transform.position = glm::vec3((rand() % 200) / 10.0f, (rand() % 200) / 10.0f, (rand() % 200) / 10.0f);
		transform.rotation = glm::vec3(rand() % 360, rand() % 360, rand() % 360);
		transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);

		sceneObjects[i]->SetTransform(transform);
	}
}

void HelloGL::LoadTextures()
{
	penguinTexture = new Texture2D();
	penguinTexture->Load((char*)"Res/Textures/penguins.raw", 512, 512);

	parrotTexture = new Texture2D();
	parrotTexture->Load((char*)"Res/Textures/parrot.bmp");

	parrotTexture32 = new Texture2D();
	parrotTexture32->Load((char*)"Res/Textures/parrot32.bmp");

	parrotTextureTGA = new Texture2D();
	parrotTextureTGA->Load((char*)"Res/Textures/parrot.tga");

	betterCubeTexture = new Texture2D();
	betterCubeTexture->Load((char*)"Res/Textures/BetterCube32.bmp");
}

void HelloGL::InitShaders()
{
	lightingShader = new Shader("Res/Shaders/VertLighting.vert", "Res/Shaders/FragLighting.frag");
	basicShader = new Shader("Res/Shaders/VertexBasic.vert", "Res/Shaders/FragBasic.frag");
}
