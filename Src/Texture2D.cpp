#include "Texture2D.h"
#include "Common.cpp"


void Texture2D::GenerateTexture()
{
	glGenTextures(1, &_TextureID);
	glBindTexture(_textureType, _TextureID);
	glTexParameteri(_textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(_textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(_textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(_textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(_textureType, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

bool Texture2D::RawLoader(std::string path, int width, int height)
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

	glTexImage2D(_textureType, 0, 3, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, tempTextureData);

	delete[] tempTextureData;
	return true;
}

bool Texture2D::BmpLoader(std::string path)
{
	std::ifstream inFile;
	unsigned int imageSize;
	int pixelArrayAddress;
	int compressionMethod;
	int bpp;
	char fileHeader[54];
	unsigned char* tempTextureData;

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

	compressionMethod = *(reinterpret_cast<int*>(&fileHeader[30]));	//gets 4 bytes from array and interprets them as 1 integer
	if(compressionMethod != 0)
	{
		std::cerr << path << " uses compression, so it isn't supported" << std::endl;
		return false;
	}

	bpp = *(reinterpret_cast<int*>(&fileHeader[28]));
	if (bpp != 24 && bpp != 32)
	{
		std::cerr << path << " isn't a 24-bit or a 32-bit bmp, so it isn't supported" << std::endl;
		return false;
	}

	pixelArrayAddress = *(reinterpret_cast<int*>(&fileHeader[10]));
	_width = *(reinterpret_cast<int*>(&fileHeader[18]));
	_height = *(reinterpret_cast<int*>(&fileHeader[22]));
	imageSize = *(reinterpret_cast<int*>(&fileHeader[34]));

	tempTextureData = new(std::nothrow) unsigned char[imageSize];
	if(tempTextureData == nullptr)
	{
		std::cerr << "Failed to allocate memory for texture" << std::endl;
		return false;
	}

	inFile.seekg(std::streampos(pixelArrayAddress));
	inFile.read((char*)tempTextureData, imageSize);
	inFile.close();

	std::cout << path << " loaded." << std::endl;

	switch (bpp) 
	{
	case 24:
		glTexImage2D(_textureType, 0, GL_RGB, _width, _height, 0, GL_BGR, GL_UNSIGNED_BYTE, tempTextureData);
		break;
	case 32:
		glTexImage2D(_textureType, 0, GL_RGBA, _width, _height, 0, GL_BGRA, GL_UNSIGNED_BYTE, tempTextureData);
		_isTransparent = true;
		break;
	};

	delete[] tempTextureData;
	return true;
}

bool Texture2D::TgaLoader(std::string path)
{
	std::ifstream inFile;
	char fileHeader[18];
	unsigned char type;
	unsigned char bpp;
	unsigned char* tempTextureData;
	int imageSize;

	inFile.open(path, std::ios::binary);

	if (!inFile.good())
	{
		std::cerr << "Can't open texture file " << path << std::endl;
		return false;
	}

	inFile.read(fileHeader, 18);

	type = fileHeader[2];
	if (type != 2)
	{
		std::cerr << path << " is not an uncompressed true-color TGA, so it is not supported" << std::endl;
		return false;
	}

	bpp = fileHeader[16];

	_width = *(reinterpret_cast<short int*>(&fileHeader[12])); //gets 2 bytes from array and interprets them as a short integer
	_height = *(reinterpret_cast<short int*>(&fileHeader[14]));
	imageSize = _width * _height * bpp / 8;

	tempTextureData = new unsigned char[imageSize];
	inFile.read((char*)tempTextureData, imageSize);
	inFile.close();

	std::cout << path << " loaded." << std::endl;

	switch (bpp)
	{
	case 24:
		glTexImage2D(_textureType, 0, GL_RGB, _width, _height, 0, GL_BGR, GL_UNSIGNED_BYTE, tempTextureData);
		break;
	case 32:
		glTexImage2D(_textureType, 0, GL_RGBA, _width, _height, 0, GL_BGRA, GL_UNSIGNED_BYTE, tempTextureData);
		_isTransparent = true;
		break;
	};

	delete[] tempTextureData;
	return true;
}

Texture2D::Texture2D(GLenum textureType) :
	_width(0), _height(0), _TextureID(0), _isTransparent(false), _textureType(textureType)
{
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &_TextureID);
}

void Texture2D::Bind()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(_textureType, _TextureID);
}

bool Texture2D::Load(std::string path, int width, int height)
{
	std::string fileExtension = GetFileExtension(path);
	
	GenerateTexture();

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
		else if (fileExtension == "tga")
			return TgaLoader(path);

		std::cout << "File type " << fileExtension << " is not supported" << std::endl;
		return false;
	}
	
	std::cout << path << " does not have a file extension " << std::endl;
	return false;
}
