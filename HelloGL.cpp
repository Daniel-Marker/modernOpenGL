#include "HelloGL.h"

#include <iostream>
#include <string>

//todo
//Sort folder structure

//Separate HelloGL constructor into InitGL and InitObjects
//Fix memory leak with vao's
//Refactor SceneObject to be a class with virtual functions and have current SceneObject be a Cube class inherited from SceneObject
//add special keys callback
//add mesh objects and meshloader namespace
//add load functionality from Cube to sceneobject
//maybe make input manager a static object
//Update Texture2D to be able to load textures from: bmp, png and raw files (raw already done)
//Add lighting
//Figure out if worth using a linked list for list/array of scene objects

HelloGL::HelloGL(int argc, char* argv[])
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

	shader = new Shader("Res/Shaders/VertexBasic.vert", "Res/Shaders/FragBasic.frag");

	inputManager = new InputManager();

	texture = new Texture2D();
	texture->Load((char*)"penguins.raw", 512, 512);

	for(int i = 0; i < 200; i++)
		pyramids[i] = new SceneObject(shader, inputManager, texture);

	camera = new Camera;
	camera->eye = glm::vec3(0.0f, 0.0f, -5.0f);
	camera->center = glm::vec3(0.0f, 0.0f, 0.0f);
	camera->up = glm::vec3(0.0f, 1.0f, 0.0f);

	viewProjMatrix = glm::mat4(1.0f);
	for (int i = 0; i < 200; i++)
	{
		pyramids[i]->transform.position = glm::vec3((rand() % 200)/10.0f, (rand() % 200) / 10.0f, (rand() % 200) / 10.0f);
		pyramids[i]->transform.rotation = glm::vec3(rand() % 360, rand() % 360, rand() % 360);
		pyramids[i]->transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
	}

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	glutMainLoop();
}


HelloGL::~HelloGL()
{
	delete camera;
	delete inputManager;
}

void HelloGL::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewProjMatrix = glm::perspective(cFOV, aspectRatio, cNearClippingPlaneDist, cFarClippingPlaneDist) *
		glm::lookAt(camera->eye,camera->center, camera->up);
	shader->SetUniformMatrix(viewProjMatrix, "u_VP");

	for(int i = 0; i < 200; i++)
		pyramids[i]->Render();

	glFlush();
	glutSwapBuffers();
}

void HelloGL::Update(float deltaTime)
{
	for(int i = 0; i < 200; i++)
		pyramids[i]->Update(deltaTime);

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
