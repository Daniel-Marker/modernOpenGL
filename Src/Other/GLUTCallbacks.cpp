#include "GLUTCallbacks.h"
#include "HelloGL.h"
#include "InputManager.h"
#include "Constants.h"

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

		if(helloGL != nullptr)
			helloGL->Update(deltaTime / cDeltaTimeDivisor);

		glutTimerFunc(preferredRefresh - deltaTime, GLUTCallbacks::Timer, preferredRefresh);
	}

	void Keyboard(unsigned char key, int x, int y)
	{
		InputManager::Keyboard(key, x, y);
	}

	void KeyboardUp(unsigned char key, int x, int y)
	{
		InputManager::KeyboardUp(key, x, y);
	}

	void WindowResize(int height, int width)
	{
		if (helloGL != nullptr)
			helloGL->OnResize(width, height);
	}

	void MotionFunc(int x, int y)
	{
		if (helloGL != nullptr)
			InputManager::MouseMovement(x, y, helloGL->GetWidth(), helloGL->GetHeight());
	}

	void PassiveMotionFunc(int x, int y)
	{
		if (helloGL != nullptr)
			InputManager::MouseMovement(x, y, helloGL->GetWidth(), helloGL->GetHeight());
	}
}