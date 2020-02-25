#include "HelloGL.h"

#include <iostream>
#include <string>

//todo
//Look back over code originating from tutorials and refactor where necessary
//add mesh objects
//add load functionality from Cube to sceneobject
//add texture class and code
//add special keys callback
//maybe make input manager a static object
//Add lighting

HelloGL::HelloGL(int argc, char* argv[]) : _cRefreshRate(16)
{
	GLUTCallbacks::Init(this);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Simple OpenGL Program");

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	std::cout << "Using OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	glutDisplayFunc(GLUTCallbacks::Display);
	glutTimerFunc(_cRefreshRate, GLUTCallbacks::Timer, _cRefreshRate);
	glutKeyboardFunc(GLUTCallbacks::Keyboard);
	glutKeyboardUpFunc(GLUTCallbacks::KeyboardUp);

	shader = new Shader("Res/Shaders/VertexBasic.vert", "Res/Shaders/FragBasic.frag");

	inputManager = new InputManager();

	for(int i = 0; i < 200; i++)
		pyramids[i] = new SceneObject(shader, inputManager);

	camera = new Camera;
	camera->eye.x = 0.0f; camera->eye.y = 0.0f; camera->eye.z = -5.0f;
	//camera->eye.x = 0.0f; camera->eye.y = 0.0f; camera->eye.z = 1.0f;
	//camera->eye.x = 5.0f; camera->eye.y = 5.0f; camera->eye.z = -5.0f;
	camera->center.x = 0.0f; camera->center.y = 0.0f; camera->center.z = 0.0f;
	camera->up.x = 0.0f; camera->up.y = 1.0f; camera->up.z = 0.0f;

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

	viewProjMatrix = glm::perspective(45.0f, 1.0f, 0.1f, 1000.0f) * glm::lookAt(
		glm::vec3(camera->eye.x, camera->eye.y, camera->eye.z),
		glm::vec3(camera->center.x, camera->center.y, camera->center.z),
		glm::vec3(camera->up.x, camera->up.y, camera->up.z));
	shader->SetUniformMatrix(viewProjMatrix, "u_VP");

	for(int i = 0; i < 200; i++)
		pyramids[i]->Render(viewProjMatrix);

	glFlush();
	glutSwapBuffers();
}

void HelloGL::Update()
{
	for(int i = 0; i < 200; i++)
		pyramids[i]->Update();

	if (inputManager->GetKeyDown('8'))
	{
		camera->eye.z += 0.1f;
	}
	else if (inputManager->GetKeyDown('2'))
	{
		camera->eye.z -= 0.1f;
	}

	glutPostRedisplay();
}