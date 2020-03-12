#include "InputManager.h"
#include "GL\freeglut.h"	//freeglut library

namespace InputManager
{
	namespace 
	{
		key_state keyarr[128] = { NOT_PUSHED };
		int oldX = 0;
		int oldY = 0;

		int _deltaX = 0;
		int _deltaY = 0;
	}

	void Keyboard(unsigned char key, int x, int y)
	{
		if (key >= 'a' && key <= 'z')
		{
			keyarr[(int)key - ('a' - 'A')] = PUSHED;
		}
		else
			keyarr[(int)key] = PUSHED;
	}

	void KeyboardUp(unsigned char key, int x, int y)
	{
		if (key >= 'a' && key <= 'z')
		{
			keyarr[(int)key - ('a' - 'A')] = NOT_PUSHED;
		}
		else
			keyarr[(int)key] = NOT_PUSHED;
	}

	void MouseMovement(int x, int y)
	{
		_deltaX = x - oldX;
		_deltaY = y - oldY;

		oldX = 1920/2;
		oldY = 1080/2;

		glutWarpPointer(1920 / 2, 1080 / 2);
	}

	bool GetKeyDown(unsigned char key)
	{
		if (key >= 'a' && key <= 'z')
		{
			return((keyarr[(int)key] == PUSHED) || (keyarr[(int)key - ('a' - 'A')] == PUSHED));
		}

		return (keyarr[(int)key] == PUSHED);
	}

	void GetMouseMovement(int& deltaX, int& deltaY)
	{
		deltaX = _deltaX;
		deltaY = _deltaY;
	}
}