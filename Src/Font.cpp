#include "Font.h"

static float vertexData[] =
{
	//positions
	-1.0f, 1.0f,
	-1.0f, -1.0f,
	1.0f, -1.0f,
	1.0f, 1.0f,

	//uv coords
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f
};

static int indices[] =
{
	0, 1, 2,
	2, 3, 0
};

Font::Font(std::string fontPath, int characterHeight, int characterWidth, char firstCharacter, Shader* shader):
	_characterHeight(characterHeight), _characterWidth(characterWidth), _firstCharacter(firstCharacter), _shader(shader)
{
	_sourceFont = new Texture2D();
	_sourceFont->Load(fontPath);

	_vao = new Vao();
	_vao->BindVao();

	BufferLayout layout;

	layoutElement positionsLayout = layoutElement(2, GL_FLOAT, false, 4);
	layoutElement textureLayout = layoutElement(2, GL_FLOAT, false, 4);

	layout.AddElement(positionsLayout);
	layout.AddElement(textureLayout);

	_vao->CreateVertexBuffer(vertexData, sizeof(vertexData), layout);
	_vao->CreateIndexBuffer(indices, sizeof(indices));
}

void Font::OutputString(std::string output, glm::vec2 position, float rotation, glm::vec2 scale, float rightScreenCoord, float topScreenCoord)
{
	int charactersPerLine = _sourceFont->GetWidth() / _characterWidth;
	float uvCharacterWidth = (float)_characterWidth / (float)_sourceFont->GetWidth();
	float uvCharacterHeight = (float)_characterHeight / (float)_sourceFont->GetHeight();
	float spacing = (cTextSpacingMultiplier * scale.x) * uvCharacterWidth;

	float rotationCentreX = position.x + output.length() * uvCharacterWidth * 0.5f;
	float rotationCentreY = position.y + uvCharacterHeight * 0.5f;
	Renderer renderer;

	glm::mat4 scaleMatrix =
		glm::scale(glm::mat4(1.0f), glm::vec3(scale.x * 0.5f * rightScreenCoord, scale.y * 0.5f * topScreenCoord, 1.0f));

	glm::mat4 rotateMatrix =
		glm::translate(glm::mat4(1.0f), glm::vec3(rotationCentreX, rotationCentreY, 0.0f)) *
		glm::eulerAngleXYZ(0.0f, 0.0f, rotation) *
		glm::translate(glm::mat4(1.0f), glm::vec3(-rotationCentreX, -rotationCentreY, 0.0f));


	for (int i = 0; i < output.size(); i++)
	{
		float bottomCoord, topCoord, leftCoord, rightCoord;
		char currentChar = output[i];

		bottomCoord = (currentChar - _firstCharacter) / charactersPerLine * uvCharacterHeight;
		topCoord = bottomCoord + uvCharacterHeight - cTextUVOffset;
		leftCoord = (currentChar - _firstCharacter) % charactersPerLine * uvCharacterWidth;
		rightCoord = leftCoord + uvCharacterWidth - cTextUVOffset;

		float newUVCoords[8] =
		{	
			leftCoord, 1.0f - bottomCoord,
			leftCoord, 1.0f - topCoord,
			rightCoord, 1.0f - topCoord,
			rightCoord, 1.0f - bottomCoord
		};

		_vao->UpdateVertexBuffer(4 * 2 * sizeof(float), sizeof(newUVCoords), &newUVCoords);

		glm::mat4 transformMatrix =
			rotateMatrix *
			glm::translate(glm::mat4(1.0f), glm::vec3(position.x + (rightCoord - leftCoord) * i * spacing, position.y, 0.0f)) *
			scaleMatrix;

		_shader->SetUniformMatrix(transformMatrix, "u_Transform");

		renderer.Render(_vao, _shader, _sourceFont);
	}
}
