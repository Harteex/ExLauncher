#include "ResourceManager.h"
#include "graphics_utils.h"

// TODO:
// * Handle wallpapers separately
// * Dynamically load wallpaper info by scanning directories and build up a list of them, then load on demand?

using namespace std;

ResourceManager::ResourceManager()
{
	renderer = NULL;
}

ResourceManager::~ResourceManager()
{
	UnloadContent();
}

SDL_Texture* ResourceManager::GetImage(string id)
{
	return images[id];
}

void ResourceManager::SetImage(string id, SDL_Texture* image)
{
	if (images.count(id) > 0)
	{
		SDL_DestroyTexture(images[id]);
		images.erase(id);
	}

	images[id] = image;
}

SDL_Texture* ResourceManager::LoadImage(string id, const char* filename)
{
	if (images.count(id) > 0)
		return images[id];

	if (renderer == NULL)
		return NULL;

	SDL_Texture* tempImg = loadImage(filename, renderer);
	if (tempImg == NULL)
		return NULL;

	images[id] = tempImg;
	return images[id];
}

SDL_Texture* ResourceManager::LoadImageForceLoad(string id, const char* filename)
{
	if (images.count(id) > 0)
	{
		SDL_DestroyTexture(images[id]);
		images.erase(id);
	}

	return LoadImage(id, filename);
}

void ResourceManager::UnloadImages()
{
	for (map<string, SDL_Texture*>::iterator i = images.begin(); i != images.end(); i++)
	{
		if (i->second != NULL)
			SDL_DestroyTexture(i->second);
	}
	images.clear();
}

TTF_Font* ResourceManager::GetTTFFont(string filename, int fontSize)
{
	string id = "";
	id += filename;
	id += "@";
	id += fontSize;

	if (ttfFonts.count(id) > 0)
		return ttfFonts[id];

	if (renderer == NULL)
		return NULL;

	// TODO make xml name mapping file, for now, hardcode common names
	if (filename == "regular")
		filename = "OpenSans-Regular.ttf";
	else if (filename == "light")
		filename = "OpenSans-Light.ttf";
	else if (filename == "semibold")
		filename = "OpenSans-Semibold.ttf";

	string path = "data/fonts/";
	path += filename;

	TTF_Font* tempFont = TTF_OpenFont(path.c_str(), fontSize);
	if (tempFont == NULL)
		return NULL;

	ttfFonts[id] = tempFont;
	return ttfFonts[id];
}

void ResourceManager::UnloadTTFFonts()
{
	for (map<string, TTF_Font*>::iterator i = ttfFonts.begin(); i != ttfFonts.end(); i++)
	{
		if (i->second != NULL)
			TTF_CloseFont(i->second);
	}
	ttfFonts.clear();
}

void ResourceManager::SetRenderer(SDL_Renderer* renderer)
{
	this->renderer = renderer;
}

void ResourceManager::UnloadContent()
{
	UnloadImages();
	UnloadTTFFonts();
}
