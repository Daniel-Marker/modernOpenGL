#pragma once
enum key_state { NOT_PUSHED, PUSHED };

class InputManager
{
private:
	key_state keyarr[128] = { NOT_PUSHED };

public:
	

	void Keyboard(unsigned char key, int x, int y);
	void KeyboardUp(unsigned char key, int x, int y);

	bool GetKeyDown(unsigned char key);
};

