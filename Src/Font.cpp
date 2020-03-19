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

void Font::OutputString(std::string output, glm::vec2 position, float rotation, glm::vec2 scale, glm::vec2 windowSize)
{
	//set uniforms
	glm::mat4 transformMatrix =
		glm::eulerAngleXYZ(0.0f, 0.0f, rotation) *
		glm::scale(glm::mat4(1.0f), glm::vec3(scale.x * 0.5f, scale.y * 0.5f, 1.0f));

	_shader->SetUniformMatrix(transformMatrix, "u_Transform");

	int charactersPerLine = _sourceFont->GetWidth() / _characterWidth;
	Renderer renderer;

	for (int i = 0; i < output.size(); i++)
	{
		char currentChar = output[i];

		float bottomCoord;
		float topCoord;
		float leftCoord;
		float rightCoord;

		float spacing = (500.0f * scale.x) *(float)_characterWidth / (float)_sourceFont->GetWidth();

		bottomCoord = (currentChar - _firstCharacter) / charactersPerLine;
		bottomCoord = (bottomCoord * (float)_characterHeight) / (float)_sourceFont->GetHeight();
		topCoord = bottomCoord + (float)_characterHeight / (float)_sourceFont->GetHeight() - 1/50.0f;


		leftCoord = (currentChar - _firstCharacter) % charactersPerLine;
		leftCoord = (leftCoord * (float)_characterWidth) / (float)_sourceFont->GetWidth();
		rightCoord = leftCoord + (float)_characterHeight / (float)_sourceFont->GetWidth() - 1/50.0f;

		float newUVCoords[8] =
		{	
			leftCoord, 1.0f - bottomCoord,
			leftCoord, 1.0f - topCoord,
			rightCoord, 1.0f - topCoord,
			rightCoord, 1.0f - bottomCoord
		};

		_vao->UpdateVertexBuffer(4 * 2 * sizeof(float), 4 * 2 * sizeof(float), &newUVCoords);

		glm::mat4 newTransformMatrix =
			glm::translate(glm::mat4(1.0f), glm::vec3(position.x + (rightCoord - leftCoord) * i * spacing, position.y, 0.0f)) * transformMatrix;

		_shader->SetUniformMatrix(newTransformMatrix, "u_Transform");

		renderer.Render(_vao, _shader, _sourceFont);
	}
}
