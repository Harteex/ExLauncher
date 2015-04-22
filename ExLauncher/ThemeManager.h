#ifndef _THEME_MANAGER_H
#define _THEME_MANAGER_H

/*********************************************/

#include <string>
/*#include <map>
#include "structures.h"
#include <SDL_ttf.h>*/

/*********************************************/

class ThemeManager
{
private:
	static std::string curTheme;
public:
	ThemeManager();
	~ThemeManager();

	static std::string ProcessPath(std::string path);
};

#endif
