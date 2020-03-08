#pragma once
enum key_state { NOT_PUSHED, PUSHED };

namespace InputManager 
{
	void Keyboard(unsigned char key, int x, int y);
	void KeyboardUp(unsigned char key, int x, int y);

	bool GetKeyDown(unsigned char key);
}