#include "ThemeManager.h"
#include <sstream>

using namespace std;

string ThemeManager::curTheme = "default";

ThemeManager::ThemeManager()
{
}

ThemeManager::~ThemeManager()
{
}

string ThemeManager::ProcessPath(string path)
{
	stringstream ss;
	if (path.substr(0, 7) == "@theme/")
	{
		ss << "data/themes/" << curTheme << "/" << path.substr(7);
		return ss.str();
	}

	return path;
}
