#include "InputManager.h"
#include "GL\freeglut.h"	//freeglut library

namespace InputManager
{
	namespace 
	{
		key_state keyarr[128] = { NOT_PUSHED };

		int _deltaX = 0;
		int _deltaY = 0;
	}

	void Keyboard(unsigned char key, int x, int y)
	{
		if (key >= 'a' && key <= 'z')
		{
			keyarr[(int)key - ('a' - 'A')] = PUSHED; //uses 'a'-'A' so that if a character is uppercase, it gets shifted down to lowercase
		}
		else
			keyarr[(int)key] = PUSHED;
	}

	void KeyboardUp(unsigned char key, int x, int y)
	{
		if (key >= 'a' && key <= 'z')
		{
			keyarr[(int)key - ('a' - 'A')] = NOT_PUSHED; //uses 'a'-'A' so that if a character is uppercase, it gets shifted down to lowercase
		}
		else
			keyarr[(int)key] = NOT_PUSHED;
	}

	void MouseMovement(int x, int y, int width, int height)
	{
		_deltaX = x - width/2;
		_deltaY = y - height/2;

		glutWarpPointer(width / 2, height / 2);
	}

	bool GetKeyDown(unsigned char key)
	{
		if (key >= 'a' && key <= 'z')
		{
			return((keyarr[(int)key] == PUSHED) || (keyarr[(int)key - ('a' - 'A')] == PUSHED)); //uses 'a'-'A' so that if a character is uppercase, it gets shifted down to lowercase
		}

		return (keyarr[(int)key] == PUSHED);
	}

	void GetMouseMovement(int& deltaX, int& deltaY)
	{
		deltaX = _deltaX;
		deltaY = _deltaY;
	}
}