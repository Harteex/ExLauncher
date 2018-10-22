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

#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#define HCENTER				1
#define VCENTER				2
#define LEFT				4
#define RIGHT				8
#define TOP					16
#define BOTTOM				32

/*********************************************/

#include <SDL.h>
#include <string>
#include <map>
#include <list>
#include "../structures.h"
#include <SDL_ttf.h>

/*********************************************/

class Graphics
{
private:
	std::map<std::string, SDL_Texture*> images;
	std::map<std::string, TTF_Font*> ttfFonts;
	SDL_Renderer* renderer;
public:
	Graphics();
	~Graphics();

	void SetRenderer(SDL_Renderer* renderer);
	void Clear(Color c);
	void Present();

	void DumpSurface(char* filename, SDL_Surface* s);

	Uint32 GetPixel(SDL_Surface *surface, int x, int y);
	void PutPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
	void GetColorComponents(Uint32 color, SDL_PixelFormat* format, Uint8& red, Uint8& green, Uint8& blue, Uint8& alpha);

	SDL_Texture* CreateTextureFromSurface(SDL_Surface* surface);
	SDL_Surface* CreateEmptySurface(int width, int height);
	SDL_Surface* ExtendSurface(SDL_Surface* srcSurface, int paddingTopBottom, int paddingLeftRight);
	SDL_Surface* CreateGradientSurface(int width, int height, SDL_Color fromColor, SDL_Color toColor);

	SDL_Texture* RenderText(const char* text, Color c, TTF_Font* font, Uint32 maxWidth);

	void DrawTexture(int x, int y, SDL_Texture* texture);
	void DrawTexture(Rectangle dstRect, SDL_Texture* texture);
	void DrawTexture(SDL_Rect* dstRect, SDL_Texture* texture);
	void DrawTexture(int x, int y, SDL_Texture* texture, int anchor);
	void DrawTexture(int x, int y, SDL_Texture* texture, int anchor, SDL_Rect* clip);
	void DrawTexture(int x, int y, SDL_Texture* texture, int anchor, SDL_Rect* clip, double angle, double scale, bool scaleAdjustPosition);
	void DrawTextureCenter(int y, SDL_Texture* texture, int areaWidth, int offsetX);

	void DrawSurface(int x, int y, SDL_Surface* source, SDL_Surface* dest);
	void DrawSurface(int x, int y, SDL_Surface* source, SDL_Surface* dest, SDL_Rect *clip);

	void FillRect(Rectangle rect, Color c);
	void FillRect(Box box, Color c);
	void FillRect(Color c);
	void FillRect(int x, int y, int w, int h, Color c);
	void FillRects(std::list<Rectangle> rects, Color c);

	void DrawRect(Rectangle rect, Color c);
	void DrawRect(Box box, Color c);
	void DrawRect(int x, int y, int w, int h, Color c);
	void DrawRect(Rectangle, int strokeWidth, Color c);

	void SetClipRect(Rectangle rect);
	void ClearClipRect();
	SDL_Surface* ClipSurface(SDL_Surface* surface, SDL_Rect* clip);

	bool FadeOutSurface(SDL_Surface* surface, int fadeLength);
	SDL_BlendMode ApplyBlendMode(SDL_Surface* surface, SDL_BlendMode newBlendMode);

	/*SDL_Texture* LoadImage(std::string id, const char* filename);
	SDL_Texture* LoadImageForceLoad(std::string id, const char* filename);
	void UnloadImages();*/

	/*BitmapFont* GetFont(string id);
	BitmapFont* CreateFont(string id, TTF_Font* buildFromFont);
	void UnloadFonts();*/

	/*TTF_Font* GetTTFFont(std::string filename, int fontSize);
	void UnloadTTFFonts();

	
	void UnloadContent();*/
};

#endif
