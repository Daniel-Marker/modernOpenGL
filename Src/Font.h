#pragma once

#include "Vao.h"
#include "Shader.h"
#include "Texture2D.h"
#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"


class Font
{
private:
	Vao* _vao;
	Shader* _shader;
	Texture2D* _sourceFont;

	int _characterHeight;
	int _characterWidth;
	char _firstCharacter;

public:
	Font(std::string fontPath, int characterHeight, int characterWidth, char firstCharacter, Shader* shader);
	void OutputString(std::string output, glm::vec2 position, float rotation, glm::vec2 scale, glm::vec2 windowSize);
};

