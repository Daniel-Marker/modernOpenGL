#include "Texture2D.h"

std::string Texture2D::GetFileExtension(std::string path)
{
	std::string strPath = path;
	std::string extension;

	std::size_t dotPosition = strPath.find_last_of(".");
	if (dotPosition != std::string::npos) {
		extension = strPath.substr(dotPosition + 1);
		
		//set the extension to be lowercase so that .raw and .RAW are both seen as valid file types
		for (int i = 0; i < extension.length(); i++)
		{
			extension[i] = std::tolower(extension[i]);
		}
	}

	return extension;
}

bool Texture2D::RawLoader(char* path, int width, int height)
{
	char* tempTextureData;
	int fileSize;
	std::ifstream inFile;

	_width = width;
	_height = height;
	inFile.open(path, std::ios::binary);

	if (!inFile.good())
	{
		std::cerr << "Can't open texture file " << path << std::endl;
		return false;
	}

	inFile.seekg(0, std::ios::end);
	fileSize = (int)inFile.tellg();
	tempTextureData = new char[fileSize];
	inFile.seekg(0, std::ios::beg);
	inFile.read(tempTextureData, fileSize);
	inFile.close();

	std::cout << path << " loaded." << std::endl;

	glGenTextures(1, &_TextureID);
	glBindTexture(GL_TEXTURE_2D, _TextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, tempTextureData);

	delete[] tempTextureData;
	return true;
}

bool Texture2D::BmpLoader(char* path)
{
	std::ifstream inFile;
	int imageSize;
	int pixelArrayAddress;
	int compressionMethod;
	int bpp;
	char fileHeader[54];
	char* tempTextureData;

	inFile.open(path, std::ios::binary);

	if (!inFile.good())
	{
		std::cerr << "Can't open texture file " << path << std::endl;
		return false;
	}

	inFile.read(fileHeader, 54);
	if (fileHeader[0] != 'B' && fileHeader[1] != 'M')
	{
		std::cerr << path << " isn't a valid bmp" << std::endl;
		return false;
	}

	compressionMethod = *(int*)&(fileHeader[30]);
	if(compressionMethod != 0)
	{
		std::cerr << path << " uses compression, so it isn't supported" << std::endl;
		return false;
	}

	bpp = *(int*)&(fileHeader[28]);
	if (bpp != 24 && bpp != 32)
	{
		std::cerr << path << " isn't a 24-bit or a 32-bit bmp, so it isn't supported" << std::endl;
		return false;
	}

	pixelArrayAddress = *(int*)&(fileHeader[10]);
	_width = *(int*)&(fileHeader[18]);
	_height = *(int*)&(fileHeader[22]);

	imageSize = _width * _height * bpp / 8;

	tempTextureData = new char[imageSize];
	inFile.seekg(std::streampos(pixelArrayAddress));
	inFile.read(tempTextureData, imageSize);
	inFile.close();

	std::cout << path << " loaded." << std::endl;

	glGenTextures(1, &_TextureID);
	glBindTexture(GL_TEXTURE_2D, _TextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	switch (bpp) 
	{
	case 24:
		glTexImage2D(GL_TEXTURE_2D, 0, bpp / 8, _width, _height, 0, GL_BGR, GL_UNSIGNED_BYTE, tempTextureData);
		break;
	case 32:
		glTexImage2D(GL_TEXTURE_2D, 0, bpp / 8, _width, _height, 0, GL_BGRA, GL_UNSIGNED_BYTE, tempTextureData);
		break;
	};

	delete[] tempTextureData;
	return true;
}

Texture2D::Texture2D() :
	_width(0), _height(0), _TextureID(0)
{
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &_TextureID);
}

void Texture2D::Bind()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _TextureID);
}

bool Texture2D::Load(char* path, int width, int height)
{
	std::string fileExtension = GetFileExtension(path);
	
	if (fileExtension != "") {
		if (fileExtension == "raw") {
			if (width != 0 && height != 0)
				return RawLoader(path, width, height);
			else
			{
				std::cout << "Image width or height set to 0" << std::endl;
				return false;
			}
		}
		else if (fileExtension == "bmp")
			return BmpLoader(path);

		std::cout << "File type " << fileExtension << " is not supported" << std::endl;
		return false;
	}
	
	std::cout << path << " does not have a file extension " << std::endl;
	return false;
}
