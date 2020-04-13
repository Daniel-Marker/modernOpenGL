#pragma once
class HelloGL;

namespace GLUTCallbacks
{
	void Init(HelloGL* gl);
	void Display();
	void Timer(int preferredRefresh);
	void Keyboard(unsigned char key, int x, int y);
	void KeyboardUp(unsigned char key, int x, int y);
	void WindowResize(int height, int width);
	void MotionFunc(int x, int y);
	void PassiveMotionFunc(int x, int y);
}