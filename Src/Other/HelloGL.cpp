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
//Loading screen for the specific scene
//Better way to control falloff for lighting

HelloGL::HelloGL(int argc, char* argv[])
{
	InitGL(argc, argv);
	InitShaders();
	
	LoadTextures();
	LoadMeshes();

	InitLights();
	InitMaterials();

	InitObjects();

	glutMainLoop();
}


HelloGL::~HelloGL()
{
	delete camera;

	for (int i = 0; i < sceneObjects.size(); i++)
	{
		delete sceneObjects[i];
	}
	sceneObjects.clear();

	delete lightingShader;
	delete basicShader;
	delete textShader;
	delete skyboxShader;
	delete fishShader;

	delete cubeMesh;
	delete emptyMesh;
	delete entranceWall1;
	delete entranceWall2;
	delete entranceWall3;
	delete outsideFence;
	delete mainWall;
	delete sideWall;
	delete penguin;

	for (int i = 0; i < sceneLights.size(); i++)
	{
		delete sceneLights[i];
	}

	delete basicMaterial;
	delete woodMaterial;

	delete mapTexture;
	delete grassTexture;
	delete fenceTexture;
	delete fishTankTexture;
	delete fishTexture;
	delete penguinTexture;
	delete fossilTexture;
	delete blankTexture;

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

	fishShader->SetUniformMatrix(viewMatrix, "u_View");
	fishShader->SetUniformMatrix(projMatrix, "u_Proj");

	skyboxShader->SetUniformMatrix(glm::mat4(glm::mat3(viewMatrix)), "u_View");
	skyboxShader->SetUniformMatrix(projMatrix, "u_Proj");

	glm::mat4 uiProjMatrix = glm::ortho(0.0f, cUISpaceRight, 0.0f, cUISpaceTop, -1.0f, 1.0f);
	textShader->SetUniformMatrix(uiProjMatrix, "u_Proj");

	for (int i = 0; i < sceneLights.size(); i++) 
	{
		std::string light = "u_Lights[]";
		light.insert(9, std::to_string(i));

		if (lightsDim)
		{
			SceneLight current = *sceneLights[i];

			current.SetDiffuseIntensity(current.GetDiffuseIntensity() * cDimMultiplier);
			current.SetAmbientIntensity(current.GetAmbientIntensity() * cDimMultiplier);
			current.SetSpecularIntensity(current.GetSpecularIntensity() * cDimMultiplier);
			
			current.SetLightUniforms(camera->GetPosition(), light, lightingShader);
			current.SetLightUniforms(camera->GetPosition(), light, fishShader);
		}
		else
		{
			sceneLights[i]->SetLightUniforms(camera->GetPosition(), light, lightingShader);
			sceneLights[i]->SetLightUniforms(camera->GetPosition(), light, fishShader);
		}
	}
	fishShader->SetUniformInt(sceneLights.size(), "u_NumLights");
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

	font->OutputString(roomString, glm::vec2(0.3f, 0.975f), 0.0f, glm::vec2(0.025f, 0.025f), cUISpaceRight, cUISpaceTop);

	glFlush();
	glutSwapBuffers();
}

void HelloGL::Update(float deltaTime)
{
	if (InputManager::GetKeyDown('i'))
		lightsDim = true;
	if (InputManager::GetKeyDown('o'))
		lightsDim = false;
	
	std::vector<SceneObject*> objects;
	for (int i = 0; i < sceneObjects.size(); i++) 
	{
		sceneObjects[i]->Update(deltaTime);
		sceneObjects[i]->GetAllObjects(objects);
	}

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
	lightsDim = false;

	sceneLights.reserve(MAX_LIGHTS);

	SceneLight* outsideLight = new SceneLight(glm::vec4(20.0f, 5.0f, 50.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f), 20.0f,
		glm::vec3(1.0f, 1.0f, 1.0f), 20.0f,
		glm::vec3(1.0f, 1.0f, 1.0f), 2.0f);

	SceneLight* fishLight = new SceneLight(glm::vec4(-52.0028f, 2.0f, -41.0197f, 1.0f),
		glm::vec3(0.41015625, 0.67578125f, 0.9453125f), 1.0f,
		glm::vec3(0.41015625, 0.67578125f, 0.9453125f), 5.0f,
		glm::vec3(1.0f, 1.0f, 1.0f), 0.0f);

	SceneLight* penguinLight = new SceneLight(glm::vec4(-37.7019f, 2.0f, -60.0197f, 1.0f),
		glm::vec3(1.0f, 0.0f, 0.0f), 1.25f,
		glm::vec3(1.0f, 1.0f, 1.0f), 2.5f,
		glm::vec3(0.0f, 0.0f, 1.0f), 2.5f);

	SceneLight* fossilLight = new SceneLight(glm::vec4(-37.7019f, 2.0f, -20.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f), 3.0f,
		glm::vec3(1.0f, 1.0f, 1.0f), 5.0f,
		glm::vec3(1.0f, 1.0f, 1.0f), 0.0f);

	sceneLights.push_back(outsideLight);
	sceneLights.push_back(fishLight);
	sceneLights.push_back(penguinLight);
	sceneLights.push_back(fossilLight);
}

void HelloGL::InitObjects()
{
	camera = new Camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.2f, 0.5f)));

	font = new Font("Res/Fonts/Press Start 2P.bmp", 32, 32, ' ', textShader);

	Cubemap* skyboxCubemap = new Cubemap("Res/Textures/top.bmp", "Res/Textures/bottom.bmp", "Res/Textures/left.bmp", "Res/Textures/right.bmp", "Res/Textures/front.bmp", "Res/Textures/back.bmp");
	skybox = new Skybox(cubeMesh, skyboxCubemap, skyboxShader);

	SceneObject painting = SceneObject(lightingShader, blankTexture, "Res/Models/Frame.obj", woodMaterial, camera,
		Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(4.0f, 4.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	SceneObject picture = SceneObject(lightingShader, blankTexture, "Res/Models/Plane.obj", basicMaterial, camera,
		Transform(glm::vec3(0.0f, 0.0f, -0.027925), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.313236f, 0.347159f, 0.268114f)),
		RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));


	SceneObject* entrance = new Room(lightingShader, mapTexture, emptyMesh, basicMaterial, camera,
		Transform(glm::vec3(0.0f, -2.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(44.8022f, 18.9423f, 8.0f), true), "Entrance", &roomString);

	SceneObject* entrance_LeftBackwall = new SceneObject(lightingShader, mapTexture, entranceWall3, basicMaterial, camera,
		Transform(glm::vec3(-12.7427f, 3.0f, -9.019724f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(7.658378f, 3.0f, 1.0f)));
	SceneObject* entrance_RightBackwall = new SceneObject(lightingShader, mapTexture, entranceWall3, basicMaterial, camera,
		Transform(glm::vec3(12.7427f, 3.0f, -9.019724f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(7.658378f, 3.0f, 1.0f)));

	SceneObject* entrance_Door = new SceneObject(lightingShader, mapTexture, "Res/Models/Entrance_Door.obj", basicMaterial, camera,
		Transform(glm::vec3(0.0f, 3.0f, 8.259536f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2.088745f, 3.0f, 0.336964f)));
	SceneObject* entrance_Floor = new SceneObject(lightingShader, mapTexture, "Res/Models/Entrance_Floor.obj", basicMaterial, camera,
		Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, -0.51f, 0.0f), glm::vec3(21.401081f, FLT_EPSILON, 9.019729f)));

	SceneObject* entrance_LeftWall1 = new SceneObject(lightingShader, mapTexture, entranceWall1, basicMaterial, camera,
		Transform(glm::vec3(-21.4011f, 3.0f, -2.640854f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 3.0f, 6.378870f)));
	SceneObject* entrance_LeftWall2 = new SceneObject(lightingShader, mapTexture, entranceWall2, basicMaterial, camera,
		Transform(glm::vec3(-19.3874f, 3.0f, 5.132866f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.997790f, 3.0f, 2.092283f)));
	SceneObject* entrance_LeftWall3 = new SceneObject(lightingShader, mapTexture, entranceWall3, basicMaterial, camera,
		Transform(glm::vec3(-9.74712f, 3.0f, 7.922576f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(7.658378f, 3.0f, 1.0f)));

	SceneObject* entrance_RightWall1 = new SceneObject(lightingShader, mapTexture, entranceWall1, basicMaterial, camera,
		Transform(glm::vec3(21.4011f, 3.0f, -2.640854f), glm::vec3(0.0f, glm::radians(180.0f), 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 3.0f, 6.378870f)));
	SceneObject* entrance_RightWall2 = new SceneObject(lightingShader, mapTexture, entranceWall2, basicMaterial, camera,
		Transform(glm::vec3(19.3874f, 3.0f, 5.132866f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.997790f, 3.0f, 2.092283f)));
	SceneObject* entrance_RightWall3 = new SceneObject(lightingShader, mapTexture, entranceWall3, basicMaterial, camera,
		Transform(glm::vec3(9.74712f, 3.0f, 7.922576f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(7.658378f, 3.0f, 1.0f)));

	SceneObject* entrance_Roof = new SceneObject(lightingShader, mapTexture, "Res/Models/Entrance_Roof.obj", basicMaterial, camera,
		Transform(glm::vec3(0.0f, 6.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, -0.51f, 0.0f), glm::vec3(21.401081f, FLT_EPSILON, 9.019729f)));

	entrance->AddChild(entrance_LeftBackwall);
	entrance->AddChild(entrance_RightBackwall);
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


	SceneObject* outside = new Room(lightingShader, mapTexture, emptyMesh, basicMaterial, camera,
		Transform(glm::vec3(0.0f, -2.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 0.0f, 10.1075f), glm::vec3(44.8022f, 29.2109f, 2.0f), true), "Outside", &roomString);

	SceneObject* outside_Floor = new SceneObject(lightingShader, grassTexture, "Res/Models/Outside_Floor.obj", basicMaterial, camera,
		Transform(glm::vec3(0.0f, 0.0f, 10.1075f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, -0.51f, 0.0f), glm::vec3(22.146629f, FLT_EPSILON, 14.277927f)));
	SceneObject* outside_LeftFence = new SceneObject(lightingShader, fenceTexture, outsideFence, basicMaterial, camera,
		Transform(glm::vec3(-22.3011f, 1.0f, 18.2439f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.098687f, 0.986865f, 14.505868f)));
	SceneObject* outside_RightFence = new SceneObject(lightingShader, fenceTexture, outsideFence, basicMaterial, camera,
		Transform(glm::vec3(22.3011f, 1.0f, 18.2439f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.098687f, 0.986865f, 14.505868f)));
	SceneObject* outside_BackFence = new SceneObject(lightingShader, fenceTexture, "Res/Models/Outside_BackFence.obj", basicMaterial, camera,
		Transform(glm::vec3(0.0f, 1.0f, 32.8489f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(22.401100f, 1.0f, 0.100000f)));

	outside->AddChild(outside_Floor);
	outside->AddChild(outside_LeftFence);
	outside->AddChild(outside_RightFence);
	outside->AddChild(outside_BackFence);
	sceneObjects.push_back(outside);


	SceneObject* mainRoom = new Room(lightingShader, mapTexture, emptyMesh, basicMaterial, camera,
		Transform(glm::vec3(0.0f, -2.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 6.0f, -42.03f), glm::vec3(20.4011f, 4.0f, 33.0103f), true), "Main Room", &roomString);

	SceneObject* main_Floor = new SceneObject(lightingShader, mapTexture, "Res/Models/Main_Floor.obj", basicMaterial, camera,
		Transform(glm::vec3(0.0f, 0.0f, -42.03f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, -0.51f, 0.0f), glm::vec3(22.146629f, FLT_EPSILON, 14.277927f)));
	SceneObject* main_Roof = new SceneObject(lightingShader, mapTexture, "Res/Models/Main_Roof.obj", basicMaterial, camera,
		Transform(glm::vec3(0.0f, 6.0f, -42.03f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(20.401100f, 2.0f, 33.010269f)));

	SceneObject* main_LeftWall1 = new SceneObject(lightingShader, mapTexture, mainWall, basicMaterial, camera,
		Transform(glm::vec3(-21.4011f, 3.0f, -24.0197f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, -0.0f, 0.0f), glm::vec3(1.0f, 3.0f, 15.0f)));
	SceneObject* main_LeftWall2 = new SceneObject(lightingShader, mapTexture, mainWall, basicMaterial, camera,
		Transform(glm::vec3(-21.4011f, 3.0f, -62.0197), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, -0.0f, 0.0f), glm::vec3(1.0f, 3.0f, 15.0f)));

	SceneObject* main_RightWall1 = new SceneObject(lightingShader, mapTexture, mainWall, basicMaterial, camera,
		Transform(glm::vec3(21.4011f, 3.0f, -24.0197f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, -0.0f, 0.0f), glm::vec3(1.0f, 3.0f, 15.0f)));
	SceneObject* main_RightWall2 = new SceneObject(lightingShader, mapTexture, mainWall, basicMaterial, camera,
		Transform(glm::vec3(21.4011f, 3.0f, -62.0197), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, -0.0f, 0.0f), glm::vec3(1.0f, 3.0f, 15.0f)));

	SceneObject* main_Backwall = new SceneObject(lightingShader, mapTexture, "Res/Models/Main_Backwall.obj", basicMaterial, camera,
		Transform(glm::vec3(0.0f, 3.0f, -76.0197f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, -0.0f, 0.0f), glm::vec3(20.401094f, 3.0f, 1.0f)));

	SceneObject* main_DisplayCase1 = new SceneObject(lightingShader, glassTexture, "Res/Models/DisplayCase.obj", basicMaterial, camera,
		Transform(glm::vec3(12.7427f, 1.5f, -15.0197f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, -0.0f, 0.0f), glm::vec3(1.0f, 1.5f, 1.0f)));
	SceneObject* main_DisplayCase2 = new SceneObject(lightingShader, glassTexture, "Res/Models/DisplayCase.obj", basicMaterial, camera,
		Transform(glm::vec3(-12.7427f, 1.5f, -15.0197f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, -0.0f, 0.0f), glm::vec3(1.0f, 1.5f, 1.0f)));
	SceneObject* main_DisplayCase3 = new SceneObject(lightingShader, glassTexture, "Res/Models/DisplayCase.obj", basicMaterial, camera,
		Transform(glm::vec3(12.7427f, 1.5f, -70.0197f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, -0.0f, 0.0f), glm::vec3(1.0f, 1.5f, 1.0f)));
	SceneObject* main_DisplayCase4 = new SceneObject(lightingShader, glassTexture, "Res/Models/DisplayCase.obj", basicMaterial, camera,
		Transform(glm::vec3(-12.7427f, 1.5f, -70.0197f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, -0.0f, 0.0f), glm::vec3(1.0f, 1.5f, 1.0f)));

	SceneObject* pyramid = new SceneObject(lightingShader, rockTexture, "Res/Models/Pyramid.obj", basicMaterial, camera,
		Transform(glm::vec3(0.0f, 0.0f, -0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, -0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	SceneObject* sphere = new SceneObject(lightingShader, rockTexture, "Res/Models/Sphere.obj", basicMaterial, camera,
		Transform(glm::vec3(0.0f, 0.0f, -0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, -0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	SceneObject* toruses = new SceneObject(lightingShader, rockTexture, "Res/Models/Toruses.obj", basicMaterial, camera,
		Transform(glm::vec3(0.0f, 0.0f, -0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, -0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	SceneObject* icosphere = new SceneObject(lightingShader, rockTexture, "Res/Models/Icosphere.obj", basicMaterial, camera,
		Transform(glm::vec3(0.0f, 0.0f, -0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, -0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	main_DisplayCase1->AddChild(pyramid);
	main_DisplayCase2->AddChild(sphere);
	main_DisplayCase3->AddChild(toruses);
	main_DisplayCase4->AddChild(icosphere);

	mainRoom->AddChild(main_Floor);
	mainRoom->AddChild(main_Roof);
	mainRoom->AddChild(main_LeftWall1);
	mainRoom->AddChild(main_LeftWall2);
	mainRoom->AddChild(main_RightWall1);
	mainRoom->AddChild(main_RightWall2);
	mainRoom->AddChild(main_Backwall);
	mainRoom->AddChild(main_DisplayCase1);
	mainRoom->AddChild(main_DisplayCase2);
	mainRoom->AddChild(main_DisplayCase3);
	mainRoom->AddChild(main_DisplayCase4);
	sceneObjects.push_back(mainRoom);


	SceneObject* sideRoom = new Room(lightingShader, mapTexture, emptyMesh, basicMaterial, camera,
		Transform(glm::vec3(0.0f, -2.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(37.9228f, 6.0f, -42.03f), glm::vec3(17.5217f, 4.48997f, 33.0103f), true), "Right side room", &roomString);

	SceneObject* side_Floor = new SceneObject(lightingShader, mapTexture, "Res/Models/Side_Floor.obj", basicMaterial, camera,
		Transform(glm::vec3(37.9655f, 0.0f, -42.03f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, -0.51f, 0.0f), glm::vec3(22.146629f, FLT_EPSILON, 14.277927f)));
	SceneObject* side_Roof = new SceneObject(lightingShader, mapTexture, "Res/Models/Side_Roof.obj", basicMaterial, camera,
		Transform(glm::vec3(37.9228f, 6.0f, -42.03f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(20.401100f, 2.0f, 33.010269f)));

	SceneObject* side_LeftWall = new SceneObject(lightingShader, mapTexture, sideWall, basicMaterial, camera,
		Transform(glm::vec3(37.7019f, 3.0f, -76.0197f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(15.300825f, 3.0f, 1.0f)));
	SceneObject* side_RightWall = new SceneObject(lightingShader, mapTexture, sideWall, basicMaterial, camera,
		Transform(glm::vec3(37.7019f, 3.0f, -10.0197f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(15.300825f, 3.0f, 1.0f)));
	SceneObject* side_BackWall = new SceneObject(lightingShader, mapTexture, "Res/Models/Side_Backwall.obj", basicMaterial, camera,
		Transform(glm::vec3(54.1661f, 2.51003f, -42.9568f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, -0.0f, 0.0f), glm::vec3(1.163331f, 3.489993f, 33.937218f)));

	Artwork* side_Painting = new Artwork(&painting,
		Transform(glm::vec3(52.9548f, 3.0f, -42.9568f), glm::vec3(0.0f, glm::radians(-90.0f), glm::radians(90.0f)), glm::vec3(4.0f, 4.0f, 1.0f)),
		&picture, grassTexture);
	Artwork* side_Painting2 = new Artwork(&painting,
		Transform(glm::vec3(52.9548f, 3.0f, -25.0f), glm::vec3(0.0f, glm::radians(-90.0f), glm::radians(90.0f)), glm::vec3(4.0f, 4.0f, 1.0f)),
		&picture, "Res/Textures/robbin.bmp");
	Artwork* side_Painting3 = new Artwork(&painting,
		Transform(glm::vec3(52.9548f, 3.0f, -60.9136f), glm::vec3(0.0f, glm::radians(-90.0f), glm::radians(90.0f)), glm::vec3(4.0f, 4.0f, 1.0f)),
		&picture, "Res/Textures/deer.bmp");
	Artwork* side_Painting4 = new Artwork(&painting,
		Transform(glm::vec3(37.7002f, 3.0f, -74.9718f), glm::vec3(0.0f, 0.0f, glm::radians(90.0f)), glm::vec3(4.0f, 4.0f, 1.0f)),
		&picture, "Res/Textures/frog.bmp");
	Artwork* side_Painting5 = new Artwork(&painting,
		Transform(glm::vec3(37.7002f, 3.0f, -11.0676f), glm::vec3(0.0f, glm::radians(-180.0f), glm::radians(90.0f)), glm::vec3(4.0f, 4.0f, 1.0f)),
		&picture, "Res/Textures/dove.bmp");

	sideRoom->AddChild(side_Floor);
	sideRoom->AddChild(side_Roof);
	sideRoom->AddChild(side_LeftWall);
	sideRoom->AddChild(side_RightWall);
	sideRoom->AddChild(side_BackWall);
	sideRoom->AddChild(side_Painting);
	sideRoom->AddChild(side_Painting2);
	sideRoom->AddChild(side_Painting3);
	sideRoom->AddChild(side_Painting4);
	sideRoom->AddChild(side_Painting5);
	sceneObjects.push_back(sideRoom);


	SceneObject* sideRoom2 = new Room(lightingShader, mapTexture, emptyMesh, basicMaterial, camera,
		Transform(glm::vec3(0.0f, -2.0f, -86.0394f), glm::vec3(0.0f, glm::radians(180.0f), 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(37.9228f, 6.0f, -42.03f), glm::vec3(17.5217f, 4.48997f, 33.0103f), true), "Left side room", &roomString);

	SceneObject* side_FishTank = new SceneObject(lightingShader, fishTankTexture, "Res/Models/FishTank.obj", basicMaterial, camera,
		Transform(glm::vec3(52.0028f, 2.0f, -46.0197f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 2.0f, 5.0f)));

	SceneObject* side_Fish = new Fish(fishShader, fishTexture, "Res/Models/fish.obj", basicMaterial, camera,
		Transform(glm::vec3(52.0028f, 2.0f, -46.0197f), glm::vec3(0.0f, glm::radians(180.0f), 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 2.0f, 5.0f)));

	SceneObject* side_Fence = new SceneObject(lightingShader, fenceTexture, outsideFence, basicMaterial, camera,
		Transform(glm::vec3(37.7019f, 0.5f, -28.0188f), glm::vec3(0.0f, glm::radians(90.0f), 0.0f), glm::vec3(1.0f, 0.5f, 1.05481f)),
		RectCollider(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.098687f, 2.0f, 14.505868f)));

	SceneObject* penguinController = new PenguinController(lightingShader, mapTexture, emptyMesh, basicMaterial, camera,
		Transform(glm::vec3(37.7019f, 0.0f, -19.01925f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	SceneObject* side_Penguin = new Penguin(lightingShader, penguinTexture, penguin, basicMaterial, camera,
		Transform(glm::vec3(2.0f, 0.0f, 2.0f), glm::vec3(0.0f, glm::radians(45.0), 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)), 0.0f);
	SceneObject* side_Penguin2 = new Penguin(lightingShader, penguinTexture, penguin, basicMaterial, camera,
		Transform(glm::vec3(-2.0f, 0.0f, 2.0f), glm::vec3(0.0f, glm::radians(90.0f), 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)), 1.0f);
	SceneObject* side_Penguin3 = new Penguin(lightingShader, penguinTexture, penguin, basicMaterial, camera,
		Transform(glm::vec3(-2.0f, 0.0f, -2.0f), glm::vec3(0.0f, glm::radians(135.0f), 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)), 2.0f);
	SceneObject* side_Penguin4 = new Penguin(lightingShader, penguinTexture, penguin, basicMaterial, camera,
		Transform(glm::vec3(2.0f, 0.0f, -2.0f), glm::vec3(0.0f, glm::radians(180.0f), 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)), 3.0f);
	penguinController->AddChild(side_Penguin);
	penguinController->AddChild(side_Penguin2);
	penguinController->AddChild(side_Penguin3);
	penguinController->AddChild(side_Penguin4);

	SceneObject* side_Fossil = new SceneObject(lightingShader, fossilTexture, "Res/Models/fossil.obj", basicMaterial, camera,
		Transform(glm::vec3(37.7019f, 1.0f, -74.9977f), glm::vec3(0.0f, glm::radians(90.0f), 0.0f), glm::vec3(1.0f, 10.0f, 10.0f)),
		RectCollider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

	sideRoom2->AddChild(new SceneObject(*((SceneObject*)side_Floor)));
	sideRoom2->AddChild(new SceneObject(*((SceneObject*)side_Roof)));
	sideRoom2->AddChild(new SceneObject(*((SceneObject*)side_LeftWall)));
	sideRoom2->AddChild(new SceneObject(*((SceneObject*)side_RightWall)));
	sideRoom2->AddChild(new SceneObject(*((SceneObject*)side_BackWall)));
	sideRoom2->AddChild(side_FishTank);
	sideRoom2->AddChild(side_Fish);
	sideRoom2->AddChild(penguinController);
	sideRoom2->AddChild(side_Fence);
	sideRoom2->AddChild(side_Fossil);
	sceneObjects.push_back(sideRoom2);
}

void HelloGL::LoadTextures()
{
	mapTexture = new Texture2D();
	mapTexture->Load("Res/Textures/mapTexture.bmp");

	grassTexture = new Texture2D();
	grassTexture->Load("Res/Textures/grass.bmp");

	fenceTexture = new Texture2D();
	fenceTexture->Load("Res/Textures/fence.bmp");

	fishTankTexture = new Texture2D();
	fishTankTexture->Load("Res/Textures/fishTank.bmp");

	fishTexture = new Texture2D();
	fishTexture->Load("Res/Textures/fish.bmp");

	penguinTexture = new Texture2D();
	penguinTexture->Load("Res/Textures/Penguin.bmp");

	fossilTexture = new Texture2D();
	fossilTexture->Load("Res/Textures/fossil.bmp");

	blankTexture = new Texture2D();
	blankTexture->Load("Res/Textures/blank.bmp");

	glassTexture = new Texture2D();
	glassTexture->Load("Res/Textures/Glass.bmp");

	rockTexture = new Texture2D();
	rockTexture->Load("Res/Textures/rock.bmp");
}

void HelloGL::LoadMeshes()
{
	cubeMesh = new Mesh("Res/Models/Cube.obj");
	emptyMesh = new Mesh("Res/Models/Empty.obj");

	entranceWall1 = new Mesh("Res/Models/Entrance_Wall1.obj");
	entranceWall2 = new Mesh("Res/Models/Entrance_Wall2.obj");
	entranceWall3 = new Mesh("Res/Models/Entrance_Wall3.obj");

	outsideFence = new Mesh("Res/Models/Outside_Fence.obj");

	mainWall = new Mesh("Res/Models/Main_Wall.obj");
	sideWall = new Mesh("Res/Models/Side_Wall.obj");

	penguin = new Mesh("Res/Models/Penguin.obj");
}

void HelloGL::InitMaterials()
{
	glm::vec3 materialAmbient = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 materialDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 materialSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
	
	basicMaterial = new Material(materialAmbient, materialDiffuse, materialSpecular, 2.0f);

	woodMaterial = new Material(glm::vec3(0.329412f, 0.184314f, 0.090196f), glm::vec3(0.329412f, 0.184314f, 0.090196f), glm::vec3(0.33f, 0.33f, 0.33f), 49.0f);
}

void HelloGL::InitShaders()
{
	lightingShader = new Shader("Res/Shaders/VertLighting.vert", "Res/Shaders/FragLighting.frag");
	basicShader = new Shader("Res/Shaders/VertexBasic.vert", "Res/Shaders/FragBasic.frag");
	textShader = new Shader("Res/Shaders/UI vertex.vert", "Res/Shaders/UI frag.frag");
	skyboxShader = new Shader("Res/Shaders/Skybox.vert", "Res/Shaders/Skybox.frag");
	fishShader = new Shader("Res/Shaders/FishShader.vert", "Res/Shaders/FragLighting.frag");
}
