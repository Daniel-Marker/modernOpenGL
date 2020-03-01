#include "HelloGL.h"

#include <iostream>
#include <string>

//todo to be up to date with tutorials:
//add load functionality for files from tutorials
//add obj mesh loading
//Add lighting

//todo after up to date with tutorials:
//Have code actually use the return value of texture load
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
	delete shader;

	delete penguinTexture;
	delete parrotTexture;
	delete parrotTexture32;

	for (int i = 0; i < 200; i++)
	{
		delete sceneObjects[i];
	}

	delete mesh;
}

void HelloGL::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewProjMatrix = glm::perspective(cFOV, aspectRatio, cNearClippingPlaneDist, cFarClippingPlaneDist) *
		glm::lookAt(camera->eye,camera->center, camera->up);
	shader->SetUniformMatrix(viewProjMatrix, "u_VP");

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
	mesh = new Mesh();

	for (int i = 0; i < 50; i++)
	{
		sceneObjects[i] = new Cube(shader, inputManager, penguinTexture, mesh);

		Transform transform;
		transform.position = glm::vec3((rand() % 200) / 10.0f, (rand() % 200) / 10.0f, (rand() % 200) / 10.0f);
		transform.rotation = glm::vec3(rand() % 360, rand() % 360, rand() % 360);
		transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);

		sceneObjects[i]->SetTransform(transform);
	}
	
	for (int i = 50; i < 100; i++)
	{
		sceneObjects[i] = new Cube(shader, inputManager, parrotTexture, mesh);

		Transform transform;
		transform.position = glm::vec3((rand() % 200) / 10.0f, (rand() % 200) / 10.0f, (rand() % 200) / 10.0f);
		transform.rotation = glm::vec3(rand() % 360, rand() % 360, rand() % 360);
		transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);

		sceneObjects[i]->SetTransform(transform);
	}

	for (int i = 100; i < 150; i++)
	{
		sceneObjects[i] = new Cube(shader, inputManager, parrotTexture32, mesh);

		Transform transform;
		transform.position = glm::vec3((rand() % 200) / 10.0f, (rand() % 200) / 10.0f, (rand() % 200) / 10.0f);
		transform.rotation = glm::vec3(rand() % 360, rand() % 360, rand() % 360);
		transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);

		sceneObjects[i]->SetTransform(transform);
	}

	for (int i = 150; i < 200; i++)
	{
		sceneObjects[i] = new Cube(shader, inputManager, parrotTextureTGA, mesh);

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

}

void HelloGL::InitShaders()
{
	shader = new Shader("Res/Shaders/VertexBasic.vert", "Res/Shaders/FragBasic.frag");
}
