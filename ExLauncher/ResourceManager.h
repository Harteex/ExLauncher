#ifndef _RESOURCE_MANAGER_H
#define _RESOURCE_MANAGER_H

/*********************************************/

#include <SDL.h>
#include <string>
#include <map>
#include "structures.h"
#include <SDL_ttf.h>

/*********************************************/

class ResourceManager
{
private:
	std::map<std::string, SDL_Texture*> images;
	std::map<std::string, TTF_Font*> ttfFonts;
	SDL_Renderer* renderer;
public:
	ResourceManager();
	~ResourceManager();
		
	SDL_Texture* GetImage(std::string id);
	void SetImage(std::string id, SDL_Texture* image);
	SDL_Texture* LoadImage(std::string id, const char* filename);
	SDL_Texture* LoadImageForceLoad(std::string id, const char* filename);
	void UnloadImages();

	/*BitmapFont* GetFont(string id);
	BitmapFont* CreateFont(string id, TTF_Font* buildFromFont);
	void UnloadFonts();*/

	TTF_Font* GetTTFFont(std::string filename, int fontSize);
	void UnloadTTFFonts();

	void SetRenderer(SDL_Renderer* renderer);
	void UnloadContent();
};

#endif
