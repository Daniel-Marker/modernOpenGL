#include "InputManager.h"

void InputManager::Keyboard(unsigned char key, int x, int y)
{
	if (key >= 'a' && key <= 'z')
	{
		keyarr[(int)key - ('a' - 'A')] = PUSHED;
	}
	else
		keyarr[(int)key] = PUSHED;
}

void InputManager::KeyboardUp(unsigned char key, int x, int y)
{
	if (key >= 'a' && key <= 'z')
	{
		keyarr[(int)key - ('a' - 'A')] = NOT_PUSHED;
	}
	else
		keyarr[(int)key] = NOT_PUSHED;
}

bool InputManager::GetKeyDown(unsigned char key)
{
	if (key >= 'a' && key <= 'z')
	{
		return((keyarr[(int)key] == PUSHED) || (keyarr[(int)key - ('a' - 'A')] == PUSHED));
	}

	return (keyarr[(int)key] == PUSHED);
}
