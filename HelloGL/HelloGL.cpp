#include "HelloGL.h"

#include <iostream>
#include <string>

//todo
//add load functionality from Cube to sceneobject
//add special keys callback
//maybe make input manager a static object

//CompileShader and CreateShader are both adapted from code in these videos https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2
static GLuint CompileShader(GLuint type, const std::string& sourcePath)
{
	GLuint shader = glCreateShader(type);

	std::string source;
	std::string line;
	std::ifstream inFile(sourcePath);

	//Check if the file failed to open
	if (inFile.fail())
	{
		std::cerr << "Can't open shader source file " << sourcePath << std::endl;
		return 0;
	}

	//Read in source code
	while (!inFile.eof())
	{
		std::getline(inFile, line);
		source = source + line + '\n';
	}
	inFile.close();

	const char* src = source.c_str();
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	int compileState;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileState);
	if(compileState == GL_FALSE)
	{
		std::cout << "Shader " << sourcePath << " failed to compile" << std::endl;

		//get the length of the error message
		int length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

		if (length > 0) {
			//create a char array and put the error message into it
			char* errorMessage;
			errorMessage = new char[length];
			glGetShaderInfoLog(shader, length, NULL, errorMessage);

			std::cout << errorMessage << std::endl;

			delete[] errorMessage;
		}
	}

	return shader;
}

static GLuint CreateShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint program;

	vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderPath);
	fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderPath);

	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	GLint result;
	glValidateProgram(program);
	glGetProgramiv(program, GL_VALIDATE_STATUS, &result);
	if (result == GL_FALSE)
	{
		std::cout << "Program failed to validate" << std::endl;

		//get the length of the error message
		int length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

		if (length > 0) {
			//create a char array and put the error message into it
			char* errorMessage;
			errorMessage = new char[length];
			glGetProgramInfoLog(program, length, NULL, errorMessage);

			std::cout << errorMessage << std::endl;

			delete[] errorMessage;
		}
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}

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

	shader = CreateShader("Res/Shaders/VertexBasic.vert", "Res/Shaders/FragBasic.frag");
	glUseProgram(shader);

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

	int location = glGetUniformLocation(shader, "u_VP");
	if (location != -1)
	{
		viewProjMatrix = glm::perspective(45.0f, 1.0f, 0.1f, 1000.0f) * glm::lookAt(
			glm::vec3(camera->eye.x, camera->eye.y, camera->eye.z),
			glm::vec3(camera->center.x, camera->center.y, camera->center.z),
			glm::vec3(camera->up.x, camera->up.y, camera->up.z));
		glUniformMatrix4fv(location, 1, GL_FALSE, &viewProjMatrix[0][0]);
	}

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