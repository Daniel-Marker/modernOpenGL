#include "InputManager.h"

namespace InputManager
{
	namespace 
	{
		key_state keyarr[128] = { NOT_PUSHED };
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

	bool GetKeyDown(unsigned char key)
	{
		if (key >= 'a' && key <= 'z')
		{
			return((keyarr[(int)key] == PUSHED) || (keyarr[(int)key - ('a' - 'A')] == PUSHED));
		}

		return (keyarr[(int)key] == PUSHED);
	}
}