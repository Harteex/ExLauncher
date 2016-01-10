#ifndef _HOME_DIRECTORY_H
#define _HOME_DIRECTORY_H

/*********************************************/

#include <string>
#include <vector>
#include <map>

/*********************************************/

namespace HomeDirectory
{
	std::string GetConfigPath();
	std::string GetDataPath();
	bool InitAndCreateDirectories();
}

#endif
