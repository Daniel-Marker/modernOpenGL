#include "InputManager.h"

void InputManager::Keyboard(unsigned char key, int x, int y)
{
	keyarr[(int)key] = PUSHED;
}

void InputManager::KeyboardUp(unsigned char key, int x, int y)
{
	keyarr[(int)key] = NOT_PUSHED;
}

bool InputManager::GetKeyDown(unsigned char key)
{
	return (keyarr[(int)key] == PUSHED);
}
