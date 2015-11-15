#ifndef _GRAPHICS_UTILS_H
#define _GRAPHICS_UTILS_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "structures.h"

#define HCENTER				1
#define VCENTER				2
#define LEFT				4
#define RIGHT				8
#define TOP					16
#define BOTTOM				32

Uint32 getPixel(SDL_Surface *surface, int x, int y);
void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
void dumpSurface(char* filename, SDL_Surface* s);
void getColorComponents(Uint32 color, SDL_PixelFormat* format, Uint8& red, Uint8& green, Uint8& blue, Uint8& alpha);
SDL_Texture* createEmptyTexture(SDL_Renderer* renderer, int width, int height);
SDL_Surface* createEmptySurface(int width, int height);
SDL_Surface* loadSurface(const char* filename);
SDL_Texture* loadImageFromBuffer(void* buffer, SDL_Renderer* renderer);
SDL_Texture* loadImage(const char* filename, SDL_Renderer* renderer);
SDL_Texture* loadImageFromMemory(void* buffer, int size, SDL_Renderer* renderer);
void drawSurface(int x, int y, SDL_Surface* source, SDL_Surface* dest);
void drawSurface(int x, int y, SDL_Surface* source, SDL_Surface* dest, SDL_Rect *clip);
void drawTexture(int x, int y, SDL_Texture* texture, SDL_Renderer* renderer);
void drawTexture(SDL_Rect* dstRect, SDL_Texture* texture, SDL_Renderer* renderer);
void drawTexture(int x, int y, SDL_Texture* texture, SDL_Renderer* renderer, int anchor);
void drawTexture(int x, int y, SDL_Texture* texture, SDL_Renderer* renderer, int anchor, SDL_Rect* clip);
void drawTexture(int x, int y, SDL_Texture* texture, SDL_Renderer* renderer, int anchor, SDL_Rect* clip, double angle, double scale, bool scaleAdjustPosition);
void drawTextureCenter(int y, SDL_Texture* texture, SDL_Renderer* renderer, int areaWidth, int offsetX);
bool setAllTransparentPixelsToColor(SDL_Surface* surface, Uint32 color);
SDL_Surface* extendSurface(SDL_Surface* srcSurface, int paddingTopBottom, int paddingLeftRight);
SDL_Surface* blurSurface(SDL_Surface* srcSurface, int radius);
//SDL_Surface* renderText(SDL_Surface* surface, TTF_Font* font, char* text, SDL_Color &color);
//SDL_Surface* renderTextUnicode(SDL_Surface* surface, TTF_Font* font, Uint16* text, SDL_Color &color);
SDL_Surface* CreateGradientSurface(int width, int height, SDL_Color fromColor, SDL_Color toColor);
SDL_Texture* CreateGradientTexture(int width, int height, Uint32 fromColor, Uint32 toColor, SDL_Renderer* renderer);
SDL_Surface* ClipSurface(SDL_Surface* surface, SDL_Rect* clip);
bool FadeOutSurface(SDL_Surface* surface, int fadeLength);
void DrawRect(SDL_Surface* dst, SDL_Rect* dstrect, Uint32 color);
void DrawRect(SDL_Surface* dst, SDL_Rect* dstrect, Uint32 color, int thickness);
SDL_Texture* drawText(const char* text, Uint8 r, Uint8 g, Uint8 b, TTF_Font* font, SDL_Renderer* renderer);
SDL_Surface* drawTextSurface(const char* text, Uint8 r, Uint8 g, Uint8 b, TTF_Font* font);
Rectangle ClipRect(Rectangle clip, Rectangle origClip);

#endif
