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

#ifndef _RESOURCE_MANAGER_H
#define _RESOURCE_MANAGER_H

/*********************************************/

#include <SDL.h>
#include <string>
#include <map>
#include "structures.h"
#include <SDL_ttf.h>

#ifdef HAS_LIBOPK
#include <opk.h>
#endif

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
#ifdef HAS_LIBOPK
	SDL_Texture* LoadImageFromOpk(std::string id, struct OPK *opk, std::string filename);
#endif
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
