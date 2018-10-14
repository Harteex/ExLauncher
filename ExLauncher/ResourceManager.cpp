/*
Copyright 2016 Andreas Bjerkeholt

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "ResourceManager.h"
#include <SDL_image.h>

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

	SDL_Texture* tempImg = LoadTexture(filename);
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

#ifdef HAS_LIBOPK
SDL_Texture* ResourceManager::LoadImageFromOpk(string id, struct OPK *opk, string filename)
{
	if (images.count(id) > 0)
	{
		SDL_DestroyTexture(images[id]);
		images.erase(id);
	}

	if (renderer == NULL)
		return NULL;

	void *buffer = NULL;
	size_t length;

	int ret = opk_extract_file(opk, filename.c_str(), &buffer, &length);
	
	if (ret < 0)
		return NULL;

	SDL_Texture* tempImg = LoadTexture(buffer, length);
	if (tempImg == NULL)
		return NULL;

	images[id] = tempImg;
	return images[id];
}
#endif

void ResourceManager::UnloadImage(string id)
{
	auto it = images.find(id);
	if (it != images.end())
	{
		if (it->second != NULL)
			SDL_DestroyTexture(it->second);

		images.erase(it);
	}
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
	else if (filename == "bold")
		filename = "OpenSans-Bold.ttf";
	else if (filename == "bolditalic")
		filename = "OpenSans-BoldItalic.ttf";
	else if (filename == "condensedbold")
		filename = "OpenSans-CondBold.ttf";
	else if (filename == "condensedlight")
		filename = "OpenSans-CondLight.ttf";
	else if (filename == "condensedlightitalic")
		filename = "OpenSans-CondLightItalic.ttf";
	else if (filename == "italic")
		filename = "OpenSans-Italic.ttf";
	else if (filename == "light")
		filename = "OpenSans-Light.ttf";
	else if (filename == "lightitalic")
		filename = "OpenSans-LightItalic.ttf";
	else if (filename == "semibold")
		filename = "OpenSans-Semibold.ttf";
	else if (filename == "semibolditalic")
		filename = "OpenSans-SemiboldItalic.ttf";

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

SDL_Surface* ResourceManager::LoadSurface(const char* filename)
{
	if (filename == NULL)
		return NULL;

	return IMG_Load(filename);
}

SDL_Texture* ResourceManager::LoadTexture(const char* filename)
{
	SDL_Surface* temp = LoadSurface(filename);

	if (temp == NULL)
		return NULL;

	int colorkey = SDL_MapRGB(temp->format, 0xff, 0x00, 0xff);
	SDL_SetColorKey(temp, SDL_TRUE, colorkey);

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, temp);

	SDL_FreeSurface(temp);

	return texture;
}

SDL_Texture* ResourceManager::LoadTexture(void* buffer, int size)
{
	SDL_RWops* sdlRw = SDL_RWFromMem(buffer, size);
	SDL_Surface *image = IMG_Load_RW(sdlRw, 1);

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);

	SDL_FreeSurface(image);

	return texture;
}