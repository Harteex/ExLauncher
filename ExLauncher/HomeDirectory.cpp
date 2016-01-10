#include "HomeDirectory.h"
#include <sys/stat.h>
#include <sstream>
#include <cstring>
#include <cstdlib>
#if defined(_WIN32)
#include <direct.h>
#endif

// http://standards.freedesktop.org/basedir-spec/basedir-spec-latest.html

std::string homeDir = "";
std::string folderName = "ExLauncher";

std::string HomeDirectory::GetConfigPath()
{
	return homeDir;
}

// TODO
std::string HomeDirectory::GetDataPath()
{
	return std::string();
}

bool DirectoryExists(std::string path)
{
#ifdef _WIN32
	struct _stat info;
	if (_stat(path.c_str(), &info) != 0)
		return false;

	return (info.st_mode & _S_IFDIR) != 0;
#else 
	struct stat info;
	if (stat(path.c_str(), &info) != 0)
		return false;

	return (info.st_mode & S_IFDIR) != 0;
#endif
}

bool HomeDirectory::InitAndCreateDirectories()
{
	std::stringstream path;

#ifdef _WIN32
	char* configDir = getenv("APPDATA");
	if (configDir == NULL)
		return false;

	path << configDir << "/" << folderName;

	if (!DirectoryExists(path.str()))
		_mkdir(path.str().c_str());
#else
	char* configDir = getenv("XDG_CONFIG_HOME");
	if (configDir == NULL || strlen(configDir) == 0)
	{
		configDir = getenv("HOME");
		if (configDir == NULL)
			return false;

		path << configDir << "/.config";
	}
	else
	{
		path << configDir;
	}

	if (!DirectoryExists(path.str()))
		mkdir(path.str().c_str(), S_IRWXU);

	path << "/" << folderName;

	if (!DirectoryExists(path.str()))
		mkdir(path.str().c_str(), S_IRWXU);
#endif

	homeDir = path.str();

	return true;
}
