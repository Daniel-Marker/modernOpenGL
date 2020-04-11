#include "HelloGL.h"

#include <iostream>
#include <string>

//todo ASAP


//todo whenever
//Have code actually use the return value of texture load
//fix repeated vertices problem, so that _vertexCount = tempVertexCount * 3, instead of _vertexCount = tempFaceCount * 3;
//auto triangulate faces in obj loader if they consist of more than 3 vertices
//Have obj loader be able to handle files with multiple objects
//add special keys callback
//Fix bug where rotating text scales wrt rotation
//Implement order-independent transparency

HelloGL::HelloGL(int argc, char* argv[])
{
	InitGL(argc, argv);
	InitShaders();
	
	LoadTextures();
	LoadMeshes();

	InitLights();
	InitMaterials();

	InitObjects();

	textRotation = 0.0f;

	glutMainLoop();
}


HelloGL::~HelloGL()
{
	delete camera;
	delete lightingShader;
	delete basicShader;
	delete textShader;
	delete skyboxShader;

	delete penguinTexture;
	delete parrotTexture;
	delete parrotTexture32;
	delete parrotTextureTGA;
	delete betterCubeTexture;
	delete glassTexture;
	delete glassTexture2;
	delete glassTexture3;

	for (int i = 0; i < sceneObjects.size(); i++)
	{
		delete sceneObjects[i];
	}
	sceneObjects.clear();

	delete cubeMesh;
	delete rectMesh;
	delete betterCubeMesh;

	for (int i = 0; i < sceneLights.size(); i++)
	{
		delete sceneLights[i];
	}

	delete basicMaterial;
	delete font;
	delete skybox;
}

void HelloGL::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 viewMatrix = camera->GetViewMatrix();
	glm::mat4 projMatrix = glm::perspective(cFOV, aspectRatio, cNearClippingPlaneDist, cFarClippingPlaneDist);
	lightingShader->SetUniformMatrix(viewMatrix, "u_View");
	lightingShader->SetUniformMatrix(projMatrix, "u_Proj");

	basicShader->SetUniformMatrix(viewMatrix, "u_View");
	basicShader->SetUniformMatrix(projMatrix, "u_Proj");

	skyboxShader->SetUniformMatrix(glm::mat4(glm::mat3(viewMatrix)), "u_View");
	skyboxShader->SetUniformMatrix(projMatrix, "u_Proj");

	glm::mat4 uiProjMatrix = glm::ortho(0.0f, cUISpaceRight, 0.0f, cUISpaceTop, -1.0f, 1.0f);
	textShader->SetUniformMatrix(uiProjMatrix, "u_Proj");

	for (int i = 0; i < sceneLights.size(); i++) 
	{
		std::string light = "u_Lights[]";
		light.insert(9, std::to_string(i));

		sceneLights[i]->SetLightUniforms(camera->GetPosition(), light, lightingShader);
	}
	lightingShader->SetUniformInt(sceneLights.size(), "u_NumLights");
	if (sceneLights.size() > MAX_LIGHTS)
		std::cerr << "Warning: there are more lights that the maximum number allowed" << std::endl;

	skybox->Render();

	std::vector<SceneObject*> transparentObjects;
	std::vector<SceneObject*> opaqueObjects;
	glm::mat4 worldTransform(1.0f);
	for (int i = 0; i < sceneObjects.size(); i++)
	{
		sceneObjects[i]->GetAllObjects(transparentObjects, opaqueObjects);
		sceneObjects[i]->SetChildrenWorldTransform(worldTransform);
	}

	for (int i = 0; i < opaqueObjects.size(); i++)
		opaqueObjects[i]->Render();

	//sorts the objects based on their distance (furthest to nearest)
	std::sort(transparentObjects.begin(), transparentObjects.end(), distanceComparison);
	for (int i = 0; i < transparentObjects.size(); i++)
		transparentObjects[i]->Render();

	font->OutputString("Room test", glm::vec2(0.3f, 0.975f), glm::radians(textRotation), glm::vec2(0.025f, 0.025f), cUISpaceRight, cUISpaceTop);

	glFlush();
	glutSwapBuffers();
}

void HelloGL::Update(float deltaTime)
{
	for(int i = 0; i < sceneObjects.size(); i++)
		sceneObjects[i]->Update(deltaTime);

	if (InputManager::GetKeyDown('b'))
	{
		Transform oldTransform = sceneObjects[0]->GetTransform();

		oldTransform.position += deltaTime * glm::vec3(-5.0f, 0.0f, 0.0f);
		sceneObjects[0]->SetTransform(oldTransform);
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

	if (InputManager::GetKeyDown('o'))
		textRotation += 45.0f * deltaTime;
	if (InputManager::GetKeyDown('p'))
		textRotation -= 45.0f * deltaTime;

	std::vector<SceneObject*> objects;
	for (int i = 0; i < sceneObjects.size(); i++)
		sceneObjects[i]->GetAllObjects(objects);

	camera->Update(deltaTime, objects);

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

	glutWarpPointer(windowWidth / 2, windowHeight / 2);
	glutMotionFunc(GLUTCallbacks::MotionFunc);
	glutPassiveMotionFunc(GLUTCallbacks::PassiveMotionFunc);
	glutSetCursor(GLUT_CURSOR_NONE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

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
	Cube* rect1;
	Cube* rect2;
	Cube* rect3;

	camera = new Camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.2f, 0.5f)));

	font = new Font("Res/Fonts/Press Start 2P.bmp", 32, 32, ' ', textShader);

	rect1 = new Cube(lightingShader, glassTexture, rectMesh, basicMaterial, camera,
		Transform(glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 3.0f, 2.0f)));

	rect2 = new Cube(lightingShader, glassTexture2, rectMesh, basicMaterial, camera,
		Transform(glm::vec3(-5.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 3.0f, 2.0f)));

	rect3 = new Cube(lightingShader, glassTexture3, rectMesh, basicMaterial, camera,
		Transform(glm::vec3(-5.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 3.0f, 2.0f)));

	rect1->AddChild(rect3);

	//sceneObjects.push_back(rect1);
	//sceneObjects.push_back(rect2);


	Cubemap* skyboxCubemap = new Cubemap("Res/Textures/top.bmp", "Res/Textures/bottom.bmp", "Res/Textures/left.bmp", "Res/Textures/right.bmp", "Res/Textures/front.bmp", "Res/Textures/back.bmp");
	skybox = new Skybox(cubeMesh, skyboxCubemap, skyboxShader);


	SceneObject* entrance = new Cube(basicShader, mapTexture, emptyMesh, basicMaterial, camera,
		Transform(glm::vec3(0.0f, -2.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

	SceneObject* entrance_Backwall = new Cube(basicShader, mapTexture, "Res/Models/Entrance_Backwall.obj", basicMaterial, camera,
		Transform(glm::vec3(0.0f, 3.0f, -9.019724f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(21.401081f, 3.0f, 0.5f)));
	SceneObject* entrance_Door = new Cube(basicShader, mapTexture, "Res/Models/Entrance_Door.obj", basicMaterial, camera,
		Transform(glm::vec3(0.0f, 3.0f, 8.259536f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2.088745f, 3.0f, 0.336964f)));
	SceneObject* entrance_Floor = new Cube(basicShader, mapTexture, "Res/Models/Entrance_Floor.obj", basicMaterial, camera,
		Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, -0.51f, 0.0f), glm::vec3(21.401081f, FLT_EPSILON, 9.019729f)));

	SceneObject* entrance_LeftWall1 = new Cube(basicShader, mapTexture, "Res/Models/Entrance_Wall1.obj", basicMaterial, camera,
		Transform(glm::vec3(-21.4011f, 3.0f, -2.640854f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 3.0f, 6.378870f)));
	SceneObject* entrance_LeftWall2 = new Cube(basicShader, mapTexture, "Res/Models/Entrance_Wall2.obj", basicMaterial, camera,
		Transform(glm::vec3(-19.3874f, 3.0f, 5.132866f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(-0.63f, -0.675344f, 0.0f), glm::vec3(1.997790f, 3.0f, 2.092283f)));
	SceneObject* entrance_LeftWall3 = new Cube(basicShader, mapTexture, "Res/Models/Entrance_Wall3.obj", basicMaterial, camera,
		Transform(glm::vec3(-9.74712f, 3.0f, 7.922576f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(7.658378f, 3.0f, 1.0f)));

	SceneObject* entrance_RightWall1 = new Cube(basicShader, mapTexture, "Res/Models/Entrance_Wall1.obj", basicMaterial, camera,
		Transform(glm::vec3(21.4011f, 3.0f, -2.640854f), glm::vec3(0.0f, glm::radians(180.0f), 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 3.0f, 6.378870f)));
	SceneObject* entrance_RightWall2 = new Cube(basicShader, mapTexture, "Res/Models/Entrance_Wall2.obj", basicMaterial, camera,
		Transform(glm::vec3(19.3874f, 3.0f, 5.132866f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(-0.63f, -0.675344f, 0.0f), glm::vec3(1.997790f, 3.0f, 2.092283f)));
	SceneObject* entrance_RightWall3 = new Cube(basicShader, mapTexture, "Res/Models/Entrance_Wall3.obj", basicMaterial, camera,
		Transform(glm::vec3(9.74712f, 3.0f, 7.922576f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(7.658378f, 3.0f, 1.0f)));

	SceneObject* entrance_Roof = new Cube(basicShader, mapTexture, "Res/Models/Entrance_Roof.obj", basicMaterial, camera,
		Transform(glm::vec3(0.0f, 6.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, -0.51f, 0.0f), glm::vec3(21.401081f, FLT_EPSILON, 9.019729f)));

	entrance->AddChild(entrance_Backwall);
	//entrance->AddChild(entrance_Door);
	entrance->AddChild(entrance_Floor);
	entrance->AddChild(entrance_LeftWall1);
	entrance->AddChild(entrance_LeftWall2);
	entrance->AddChild(entrance_LeftWall3);
	entrance->AddChild(entrance_RightWall1);
	entrance->AddChild(entrance_RightWall2);
	entrance->AddChild(entrance_RightWall3);
	entrance->AddChild(entrance_Roof);
	sceneObjects.push_back(entrance);
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

	glassTexture2 = new Texture2D();
	glassTexture2->Load("Res/Textures/glass2.bmp");

	glassTexture3 = new Texture2D();
	glassTexture3->Load("Res/Textures/glass3.bmp");

	mapTexture = new Texture2D();
	mapTexture->Load("Res/Textures/mapTexture.bmp");
}

void HelloGL::LoadMeshes()
{
	cubeMesh = new Mesh("Res/Models/Cube.obj");
	rectMesh = new Mesh("Res/Models/Rect.obj");
	betterCubeMesh = new Mesh("Res/Models/BetterCube.obj");
	emptyMesh = new Mesh("Res/Models/Empty.obj");
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
	textShader = new Shader("Res/Shaders/UI vertex.vert", "Res/Shaders/UI frag.frag");
	skyboxShader = new Shader("Res/Shaders/Skybox.vert", "Res/Shaders/Skybox.frag");
}
