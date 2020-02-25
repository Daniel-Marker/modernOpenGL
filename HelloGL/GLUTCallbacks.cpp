#include "GLUTCallbacks.h"
#include "HelloGL.h"

//Namespace implementation
namespace GLUTCallbacks
{
	namespace
	{
		//Initialise to a null pointer before we do anything
		HelloGL* helloGL = nullptr;
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
		helloGL->Update();
		updateTime = glutGet(GLUT_ELAPSED_TIME) - updateTime;

		glutTimerFunc(preferredRefresh - updateTime, GLUTCallbacks::Timer, preferredRefresh);
	}

	void Keyboard(unsigned char key, int x, int y)
	{
		//helloGL->Keyboard(key, x, y);
		helloGL->inputManager->Keyboard(key, x, y);
	}

	void KeyboardUp(unsigned char key, int x, int y)
	{
		//helloGL->KeyboardUp(key, x, y);
		helloGL->inputManager->KeyboardUp(key, x, y);
	}
}