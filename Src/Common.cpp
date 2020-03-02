#include <string>

static std::string GetFileExtension(std::string path)
{
	std::string strPath = path;
	std::string extension;

	std::size_t dotPosition = strPath.find_last_of(".");
	if (dotPosition != std::string::npos) {
		extension = strPath.substr(dotPosition + 1);

		//set the extension to be lowercase so that .raw and .RAW are both seen as valid file types
		for (unsigned int i = 0; i < extension.length(); i++)
		{
			extension[i] = std::tolower(extension[i]);
		}
	}

	return extension;
}