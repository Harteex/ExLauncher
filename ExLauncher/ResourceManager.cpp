#include "ResourceManager.h"
#include "graphics.h"

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

/*BitmapFont* ResourceManager::GetFont(string id)
{
	return fonts[id]; // TODO kör find istället eftersom denna skapar upp ett nullvärde?? eller är det bry?
}

BitmapFont* ResourceManager::CreateFont(string id, TTF_Font* buildFromFont)
{
	if (fonts.count(id) > 0)
	{
		delete fonts[id];
		fonts.erase(id);
	}

	BitmapFont* font = new BitmapFont();
	if (!font->BuildBitmapFont(buildFromFont, renderer))
	{
		delete font;
		return NULL;
	}
	
	fonts[id] = font;
	return font;
}

void ResourceManager::UnloadFonts()
{
	for (map<string, BitmapFont*>::iterator i = fonts.begin(); i != fonts.end(); i++)
	{
		if (i->second != NULL)
			delete (i->second);
	}
	fonts.clear();
}*/

TTF_Font* ResourceManager::GetTTFFont(string id)
{
	return ttfFonts[id];
}

TTF_Font* ResourceManager::LoadTTFFont(string id, const char* filename, int fontSize)
{
	if (ttfFonts.count(id) > 0)
		return ttfFonts[id];

	if (renderer == NULL)
		return NULL;

	TTF_Font* tempFont = TTF_OpenFont(filename, fontSize);
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
	//UnloadFonts();
	UnloadTTFFonts();
}
