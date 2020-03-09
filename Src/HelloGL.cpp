#include "HelloGL.h"

#include <iostream>
#include <string>

//todo ASAP
//Update mesh to use a vector instead of dynamic array

//todo whenever
//Have code actually use the return value of texture load
//fix repeated vertices problem, so that _vertexCount = tempVertexCount * 3, instead of _vertexCount = tempFaceCount * 3;
//auto triangulate faces in obj loader if they consist of more than 3 vertices
//Have obj loader be able to handle files with multiple objects
//add special keys callback

HelloGL::HelloGL(int argc, char* argv[])
{
	InitGL(argc, argv);
	InitShaders();
	
	LoadTextures();
	LoadMeshes();

	InitLights();
	InitMaterials();

	InitObjects();

	camera = new Camera;
	camera->eye = glm::vec3(0.0f, 0.0f, -5.0f);
	camera->center = glm::vec3(0.0f, 0.0f, 0.0f);
	camera->up = glm::vec3(0.0f, 1.0f, 0.0f);

	glutMainLoop();
}


HelloGL::~HelloGL()
{
	delete camera;
	delete lightingShader;

	delete penguinTexture;
	delete parrotTexture;
	delete parrotTexture32;
	delete parrotTextureTGA;
	delete betterCubeTexture;

	for (int i = 0; i < sceneObjects.size(); i++)
	{
		delete sceneObjects[i];
	}
	sceneObjects.clear();

	delete cubeMesh;
	delete betterCubeMesh;

	for (int i = 0; i < sceneLights.size(); i++)
	{
		delete sceneLights[i];
	}
}

void HelloGL::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 viewMatrix = glm::lookAt(camera->eye, camera->center, camera->up);
	glm::mat4 projMatrix = glm::perspective(cFOV, aspectRatio, cNearClippingPlaneDist, cFarClippingPlaneDist);
	lightingShader->SetUniformMatrix(viewMatrix, "u_View");
	lightingShader->SetUniformMatrix(projMatrix, "u_Proj");

	basicShader->SetUniformMatrix(viewMatrix, "u_View");
	basicShader->SetUniformMatrix(projMatrix, "u_Proj");

	for (int i = 0; i < sceneLights.size(); i++) 
	{
		std::string light = "u_Lights[]";
		light.insert(9, std::to_string(i));

		sceneLights[i]->SetLightUniforms(camera->center, light, lightingShader);
	}
	lightingShader->SetUniformInt(sceneLights.size(), "u_NumLights");
	if (sceneLights.size() > MAX_LIGHTS)
		std::cerr << "Warning: there are more lights that the maximum number allowed" << std::endl;

	std::vector<SceneObject*> transparentObjects;
	for (int i = 0; i < sceneObjects.size(); i++)
	{
		if(!sceneObjects[i]->GetTransparent())
			sceneObjects[i]->Render();
		else 
			transparentObjects.push_back(sceneObjects[i]);
		//create a separate list for transparent objects
	}

	//as < is defined for sceneObjects as whichever object has the smallest distance to the camera, this creates a list of transparent objects from nearest to farthest
	std::sort(transparentObjects.begin(), transparentObjects.end());
	//so we need to reverse it before rendering
	std::reverse(transparentObjects.begin(), transparentObjects.end());
	for (int i = 0; i < transparentObjects.size(); i++)
	{
		transparentObjects[i]->Render();
	}

	glFlush();
	glutSwapBuffers();
}

void HelloGL::Update(float deltaTime)
{
	for(int i = 0; i < sceneObjects.size(); i++)
		sceneObjects[i]->Update(deltaTime);

	if (InputManager::GetKeyDown('8'))
	{
		camera->eye.z += 5.0f * deltaTime;
	}
	else if (InputManager::GetKeyDown('2'))
	{
		camera->eye.z -= 5.0f * deltaTime;
	}

	if (InputManager::GetKeyDown('d'))
	{
		camera->center.x -= 5.0f * deltaTime;
		camera->eye.x -= 5.0f * deltaTime;
	}

	if (InputManager::GetKeyDown('a'))
	{
		camera->center.x += 5.0f * deltaTime;
		camera->eye.x += 5.0f * deltaTime;
	}

	if (InputManager::GetKeyDown('w'))
	{
		camera->center.y += 5.0f * deltaTime;
		camera->eye.y += 5.0f * deltaTime;
	}

	if (InputManager::GetKeyDown('s'))
	{
		camera->center.y -= 5.0f * deltaTime;
		camera->eye.y -= 5.0f * deltaTime;
	}


	if (InputManager::GetKeyDown('i'))
	{
		for(int i = 0; i < sceneLights.size(); i++)
			sceneLights[i]->SetPosition(sceneLights[i]->GetPosition() + glm::vec4(0.0f, 5.0f, 0.0f, 0.0f) * deltaTime);
	}
	if (InputManager::GetKeyDown('j'))
	{
		for (int i = 0; i < sceneLights.size(); i++)
			sceneLights[i]->SetPosition(sceneLights[i]->GetPosition() + glm::vec4(5.0f, 0.0f, 0.0f, 0.0f) * deltaTime);
	}
	if (InputManager::GetKeyDown('k'))
	{
		for (int i = 0; i < sceneLights.size(); i++)
			sceneLights[i]->SetPosition(sceneLights[i]->GetPosition() + glm::vec4(0.0f, -5.0f, 0.0f, 0.0f) * deltaTime);
	}
	if (InputManager::GetKeyDown('l'))
	{
		for (int i = 0; i < sceneLights.size(); i++)
			sceneLights[i]->SetPosition(sceneLights[i]->GetPosition() + glm::vec4(-5.0f, 0.0f, 0.0f, 0.0f) * deltaTime);
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

void HelloGL::InitLights()
{
	sceneLights.reserve(MAX_LIGHTS);

	glm::vec3 diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
	float diffuseIntensity = 2.0f;
	glm::vec3 ambientColor = glm::vec3(1.0f, 1.0f, 1.0f);
	float ambientIntensity = 2.0f;
	glm::vec3 specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
	float specularIntensity = 1.0f;

	glm::vec4 position = glm::vec4(-20.0f, 0.0f, -5.0f, 1.0f);
	sceneLights.push_back(new SceneLight(position, diffuseColor, diffuseIntensity, ambientColor, ambientIntensity, specularColor, specularIntensity));

	position = glm::vec4(-10.0f, 20.0f, -5.0f, 1.0f);
	diffuseIntensity = 15.0f;
	diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
	sceneLights.push_back(new SceneLight(position, diffuseColor, diffuseIntensity, ambientColor, ambientIntensity, specularColor, specularIntensity));
}

void HelloGL::InitObjects()
{
	sceneObjects.push_back(new Cube(lightingShader, glassTexture, cubeMesh, basicMaterial, camera));
	Transform transform;
	transform.position = glm::vec3(5.0f, 5.0f, -3.0f);
	transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	transform.scale = glm::vec3(10.0f, 10.0f, 1.0f);
	sceneObjects[0]->SetTransform(transform);

	for (int i = 1; i < 50; i++)
	{
		sceneObjects.push_back(new Cube(lightingShader, betterCubeTexture, betterCubeMesh, basicMaterial, camera));

		Transform transform;
		transform.position = glm::vec3((rand() % 200) / 10.0f, (rand() % 200) / 10.0f, (rand() % 200) / 10.0f);
		transform.rotation = glm::vec3(rand() % 360, rand() % 360, rand() % 360);
		transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);

		sceneObjects[i]->SetTransform(transform);
	}
	
	for (int i = 50; i < 100; i++)
	{
		sceneObjects.push_back(new Cube(lightingShader, parrotTexture, cubeMesh, basicMaterial, camera));

		Transform transform;
		transform.position = glm::vec3((rand() % 200) / 10.0f, (rand() % 200) / 10.0f, (rand() % 200) / 10.0f);
		transform.rotation = glm::vec3(rand() % 360, rand() % 360, rand() % 360);
		transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);

		sceneObjects[i]->SetTransform(transform);
	}

	for (int i = 100; i < 150; i++)
	{
		sceneObjects.push_back(new Cube(lightingShader, parrotTexture, cubeMesh, basicMaterial, camera));

		Transform transform;
		transform.position = glm::vec3((rand() % 200) / 10.0f, (rand() % 200) / 10.0f, (rand() % 200) / 10.0f);
		transform.rotation = glm::vec3(rand() % 360, rand() % 360, rand() % 360);
		transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);

		sceneObjects[i]->SetTransform(transform);
	}

	for (int i = 150; i < 200; i++)
	{
		sceneObjects.push_back(new Cube(basicShader, penguinTexture, cubeMesh, basicMaterial, camera));

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
	penguinTexture->Load("Res/Textures/penguins.raw", 512, 512);

	parrotTexture = new Texture2D();
	parrotTexture->Load("Res/Textures/parrot.bmp");

	parrotTexture32 = new Texture2D();
	parrotTexture32->Load("Res/Textures/parrot32.bmp");

	parrotTextureTGA = new Texture2D();
	parrotTextureTGA->Load("Res/Textures/parrot.tga");

	betterCubeTexture = new Texture2D();
	betterCubeTexture->Load("Res/Textures/BetterCube32.bmp");

	glassTexture = new Texture2D();
	glassTexture->Load("Res/Textures/glass.bmp");
}

void HelloGL::LoadMeshes()
{
	cubeMesh = new Mesh("Res/Models/Cube.obj");
	betterCubeMesh = new Mesh("Res/Models/BetterCube.obj");
}

void HelloGL::InitMaterials()
{
	glm::vec3 materialAmbient = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 materialDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 materialSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
	
	basicMaterial = new Material(materialAmbient, materialDiffuse, materialSpecular, 2.0f);
}

void HelloGL::InitShaders()
{
	lightingShader = new Shader("Res/Shaders/VertLighting.vert", "Res/Shaders/FragLighting.frag");
	basicShader = new Shader("Res/Shaders/VertexBasic.vert", "Res/Shaders/FragBasic.frag");
}
