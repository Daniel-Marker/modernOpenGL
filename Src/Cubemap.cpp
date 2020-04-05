#include "Cubemap.h"
#include <vector>

Cubemap::Cubemap(std::string top, std::string bottom, std::string left, std::string right, std::string front, std::string back)
{
	_textureType = GL_TEXTURE_CUBE_MAP;
	GenerateTexture();

	std::vector<std::string> texturePaths;
	texturePaths.push_back(right);
	texturePaths.push_back(left);
	texturePaths.push_back(bottom);
	texturePaths.push_back(top);
	texturePaths.push_back(front);
	texturePaths.push_back(back);

	for (int i = 0; i < texturePaths.size(); i++)
	{
		std::string fileExtension = GetFileExtension(texturePaths[i]);
		_textureType = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;


		if (fileExtension != "") {
			if (fileExtension == "bmp")
				BmpLoader(texturePaths[i]);
			else if (fileExtension == "tga")
				TgaLoader(texturePaths[i]);
			else
				std::cout << "File type " << fileExtension << " is not supported" << std::endl;
		}
		else
			std::cout << texturePaths[i] << " does not have a file extension " << std::endl;
	}

	_textureType = GL_TEXTURE_CUBE_MAP;
}
