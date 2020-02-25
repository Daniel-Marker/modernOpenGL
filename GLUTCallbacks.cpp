#include "GLUTCallbacks.h"
#include "HelloGL.h"

#include <iostream>

//Namespace implementation
namespace GLUTCallbacks
{
	namespace
	{
		HelloGL* helloGL = nullptr;
		float deltaTime = 0;
		int timeSinceLastFrame = 0;
	}

	void Init(HelloGL* gl)
	{
		helloGL = gl;
	}

	void Display()
	{
		if (helloGL != nullptr)
			helloGL->Display();
	}

	void Timer(int preferredRefresh)
	{
		int updateTime = glutGet(GLUT_ELAPSED_TIME);
		deltaTime = updateTime - timeSinceLastFrame;
		timeSinceLastFrame = updateTime;

		helloGL->Update(deltaTime / 1000.0f);

		glutTimerFunc(preferredRefresh - deltaTime, GLUTCallbacks::Timer, preferredRefresh);
	}

	void Keyboard(unsigned char key, int x, int y)
	{
		helloGL->inputManager->Keyboard(key, x, y);
	}

	void KeyboardUp(unsigned char key, int x, int y)
	{
		helloGL->inputManager->KeyboardUp(key, x, y);
	}
}