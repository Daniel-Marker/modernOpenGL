#pragma once

#include "Texture2D.h"
#include "Common.cpp"

class Cubemap :
	public Texture2D
{
private:


public:
	Cubemap(std::string top, std::string bottom, std::string left, std::string right, std::string front, std::string back);

};

