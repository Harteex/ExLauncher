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

#include "Graphics.h"
#include <algorithm>

Graphics::Graphics()
{
}

Graphics::~Graphics()
{
}

void Graphics::SetRenderer(SDL_Renderer* renderer)
{
	this->renderer = renderer;
}

void Graphics::Clear(Color c)
{
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
}

void Graphics::Present()
{
	SDL_RenderPresent(renderer);
}

void Graphics::DumpSurface(char* filename, SDL_Surface* s)
{
	FILE* f = fopen(filename, "w");
	for (int y = 0; y < s->h; y++)
	{
		for (int x = 0; x < s->w; x++)
		{
			Uint32 px = GetPixel(s, x, y);
			fwrite(&px, sizeof(Uint32), 1, f);
		}
	}
	fclose(f);
}

Uint32 Graphics::GetPixel(SDL_Surface *surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	// Here p is the address to the pixel we want to retrieve
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp)
	{
	case 1:
		return *p;
		break;
	case 2:
		return *(Uint16 *)p;
		break;
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;
		break;
	case 4:
		return *(Uint32 *)p;
		break;
	default:
		return 0; // shouldn't happen, but avoids warnings
	}
}

void Graphics::PutPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	int bpp = surface->format->BytesPerPixel;
	// Here p is the address to the pixel we want to set
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp)
	{
	case 1:
		*p = pixel;
		break;
	case 2:
		*(Uint16 *)p = pixel;
		break;
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
		{
			p[0] = (pixel >> 16) & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = pixel & 0xff;
		}
		else
		{
			p[0] = pixel & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = (pixel >> 16) & 0xff;
		}
		break;
	case 4:
		*(Uint32 *)p = pixel;
		break;
	}
}

void Graphics::GetColorComponents(Uint32 color, SDL_PixelFormat* format, Uint8& red, Uint8& green, Uint8& blue, Uint8& alpha)
{
	Uint32 temp;

	// Get Red component
	temp = color & format->Rmask;  // Isolate red component
	temp = temp >> format->Rshift; // Shift it down to 8-bit
	temp = temp << format->Rloss;  // Expand to a full 8-bit number
	red = (Uint8)temp;

	// Get Green component
	temp = color & format->Gmask;  // Isolate green component
	temp = temp >> format->Gshift; // Shift it down to 8-bit
	temp = temp << format->Gloss;  // Expand to a full 8-bit number
	green = (Uint8)temp;

	// Get Blue component
	temp = color & format->Bmask;  // Isolate blue component
	temp = temp >> format->Bshift; // Shift it down to 8-bit
	temp = temp << format->Bloss;  // Expand to a full 8-bit number
	blue = (Uint8)temp;

	// Get Alpha component
	temp = color & format->Amask;  // Isolate alpha component
	temp = temp >> format->Ashift; // Shift it down to 8-bit
	temp = temp << format->Aloss;  // Expand to a full 8-bit number
	alpha = (Uint8)temp;
}

SDL_Texture* Graphics::CreateTextureFromSurface(SDL_Surface* surface)
{
	return SDL_CreateTextureFromSurface(renderer, surface);
}

// FIXME can we do this better? like create format after screen? or at least make sure we handle conversion to texture ok when we create like this
SDL_Surface* Graphics::CreateEmptySurface(int width, int height)
{
	SDL_Surface *surface;
	Uint32 rmask, gmask, bmask, amask;

	// SDL interprets each pixel as a 32-bit number, so our masks must depend on the endianness (byte order) of the machine
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	surface = SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);
	return surface;
}

SDL_Surface* Graphics::ExtendSurface(SDL_Surface* srcSurface, int paddingTopBottom, int paddingLeftRight)
{
	SDL_Surface* oldSurface = srcSurface;

	if (oldSurface->format->format != SDL_PIXELFORMAT_ARGB8888)
		oldSurface = SDL_ConvertSurfaceFormat(srcSurface, SDL_PIXELFORMAT_ARGB8888, 0);

	SDL_Surface* newSurface = SDL_CreateRGBSurfaceWithFormat(0, srcSurface->w + paddingLeftRight * 2, srcSurface->h + paddingTopBottom * 2, 32, SDL_PIXELFORMAT_ARGB8888);

	if (newSurface == NULL)
		return NULL;

	SDL_FillRect(newSurface, NULL, 0x00000000);

	auto oldBlendMode = ApplyBlendMode(oldSurface, SDL_BLENDMODE_NONE);
	DrawSurface(paddingLeftRight, paddingTopBottom, oldSurface, newSurface);
	ApplyBlendMode(oldSurface, oldBlendMode);

	if (oldSurface != srcSurface)
		SDL_FreeSurface(oldSurface);

	return newSurface;
}

// FIXME TEST!!
SDL_Surface* Graphics::CreateGradientSurface(int width, int height, SDL_Color fromColor, SDL_Color toColor)
{
	SDL_Surface* surface = CreateEmptySurface(width, height);
	if (surface == NULL)
		return NULL;


	//Uint8 fromR, fromG, fromB, fromA;
	//Uint8 toR, toG, toB, toA;
	int dR, dG, dB, dA;
	int addR, addG, addB, addA;

	/*getColorComponents(fromColor, surface->format, fromR, fromG, fromB, fromA);
	getColorComponents(toColor, surface->format, toR, toG, toB, toA);*/

	dR = (toColor.r - fromColor.r);
	dG = (toColor.g - fromColor.g);
	dB = (toColor.b - fromColor.b);
	dA = (toColor.a - fromColor.a);

	SDL_Rect r;
	r.x = 0;
	r.y = 0;
	r.w = 1;
	r.h = height;
	for (int i = 0; i < width; i++)
	{
		addR = (dR * i) / width;
		addG = (dG * i) / width;
		addB = (dB * i) / width;
		addA = (dA * i) / width;

		r.x = i;

		Uint32 curCol = SDL_MapRGBA(surface->format, fromColor.r + addR, fromColor.g + addG, fromColor.b + addB, fromColor.a + addA);
		SDL_FillRect(surface, &r, curCol);
	}

	return surface;
}

SDL_Texture* Graphics::RenderText(const char* text, Color c, TTF_Font* font, Uint32 maxWidth)
{
	SDL_Color fontColor;
	fontColor.r = c.r;
	fontColor.g = c.g;
	fontColor.b = c.b;
	fontColor.a = c.a;
	SDL_Surface* surfaceText = TTF_RenderText_Blended_Wrapped(font, text, fontColor, maxWidth);
	if (surfaceText == NULL)
		return NULL;

	SDL_Texture* textImage = SDL_CreateTextureFromSurface(renderer, surfaceText);
	SDL_FreeSurface(surfaceText);
	if (textImage == NULL)
		return NULL;

	return textImage;
}

void Graphics::DrawTexture(int x, int y, SDL_Texture* texture)
{
	DrawTexture(x, y, texture, TOP | LEFT, NULL);
}

void Graphics::DrawTexture(Rectangle dstRect, SDL_Texture* texture)
{
	SDL_Rect r;
	r.x = dstRect.x;
	r.y = dstRect.y;
	r.w = dstRect.w;
	r.h = dstRect.h;

	SDL_RenderCopy(renderer, texture, NULL, &r);
}

void Graphics::DrawTexture(SDL_Rect* dstRect, SDL_Texture* texture)
{
	SDL_RenderCopy(renderer, texture, NULL, dstRect);
}

void Graphics::DrawTexture(int x, int y, SDL_Texture* texture, int anchor)
{
	DrawTexture(x, y, texture, anchor, NULL);
}

void Graphics::DrawTexture(int x, int y, SDL_Texture* texture, int anchor, SDL_Rect* clip)
{
	DrawTexture(x, y, texture, anchor, clip, 0.0, 1.0, false);
}

void Graphics::DrawTexture(int x, int y, SDL_Texture* texture, int anchor, SDL_Rect* clip, double angle, double scale, bool scaleAdjustPosition)
{
	int w;
	int h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	if ((anchor & HCENTER) > 0)
	{
		x = x - w / 2;
	}
	else if ((anchor & RIGHT) > 0)
	{
		x = x - w;
	}

	if ((anchor & VCENTER) > 0)
	{
		y = y - h / 2;
	}
	else if ((anchor & BOTTOM) > 0)
	{
		y = y - h;
	}

	SDL_Rect coords;
	coords.x = x;
	coords.y = y;

	if (clip != NULL)
	{
		coords.w = clip->w;
		coords.h = clip->h;
	}
	else
	{
		//SDL_QueryTexture(texture, NULL, NULL, &coords.w, &coords.h);
		coords.w = w;
		coords.h = h;
	}

	if (scale != 1.0)
	{
		int oldW, oldH;
		oldW = coords.w;
		oldH = coords.h;

		coords.w *= scale;
		coords.h *= scale;

		// If scale adjust position is set, we move the texture such as if it would've been drawn from the center position
		if (scaleAdjustPosition)
		{
			int dW = coords.w - oldW;
			int dH = coords.h - oldH;

			if ((anchor & LEFT) > 0)
				coords.x -= dW / 2;

			if ((anchor & RIGHT) > 0)
				coords.x += dW / 2;

			if ((anchor & TOP) > 0)
				coords.y -= dH / 2;

			if ((anchor & BOTTOM) > 0)
				coords.y += dH / 2;
		}
	}

	if (angle == 0.0)
		SDL_RenderCopy(renderer, texture, clip, &coords);
	else
		SDL_RenderCopyEx(renderer, texture, clip, &coords, angle, NULL, SDL_FLIP_NONE);
}

void Graphics::DrawTextureCenter(int y, SDL_Texture* texture, int areaWidth, int offsetX)
{
	int w;
	SDL_QueryTexture(texture, NULL, NULL, &w, NULL);

	int x = (areaWidth / 2) - (w / 2) + offsetX;

	DrawTexture(x, y, texture);
}

void Graphics::DrawSurface(int x, int y, SDL_Surface* source, SDL_Surface* dest)
{
	DrawSurface(x, y, source, dest, NULL);
}

void Graphics::DrawSurface(int x, int y, SDL_Surface* source, SDL_Surface* dest, SDL_Rect* clip)
{
	SDL_Rect coords;
	coords.x = x;
	coords.y = y;

	SDL_BlitSurface(source, clip, dest, &coords);
}

void Graphics::FillRect(Rectangle rect, Color c)
{
	FillRect(rect.x, rect.y, rect.w, rect.h, c);
}

void Graphics::FillRect(Box box, Color c)
{
	FillRect(box.left, box.top, box.right - box.left, box.bottom - box.top, c);
}

void Graphics::FillRect(Color c)
{
	if (c.a != 0xff)
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
	SDL_RenderFillRect(renderer, NULL);

	if (c.a != 0xff)
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void Graphics::FillRect(int x, int y, int w, int h, Color c)
{
	if (c.a != 0xff)
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);

	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;

	SDL_RenderFillRect(renderer, &r);

	if (c.a != 0xff)
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

// FIXME UNTESTED
void Graphics::FillRects(std::list<Rectangle> rects, Color c)
{
	int numRects = rects.size();

	SDL_Rect* sdlRects = new SDL_Rect[numRects];
	int i = 0;
	for (Rectangle r : rects)
	{
		sdlRects[i].x = r.x;
		sdlRects[i].y = r.y;
		sdlRects[i].w = r.w;
		sdlRects[i].h = r.h;

		i++;
	}

	SDL_RenderFillRects(renderer, sdlRects, numRects);
	delete [] sdlRects;
}

void Graphics::DrawRect(Rectangle rect, Color c)
{
	DrawRect(rect.x, rect.y, rect.w, rect.h, c);
}

void Graphics::DrawRect(Box box, Color c)
{
	DrawRect(box.left, box.top, box.right - box.left, box.bottom - box.top, c);
}

void Graphics::DrawRect(int x, int y, int w, int h, Color c)
{
	if (c.a != 0xff)
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);

	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;

	SDL_RenderDrawRect(renderer, &r);

	if (c.a != 0xff)
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

// FIXME untested
void Graphics::DrawRect(Rectangle rect, int strokeWidth, Color c)
{
	if (strokeWidth <= 0)
	{
		return;
	}
	else if (strokeWidth == 1)
	{
		DrawRect(rect, c);
		return;
	}

	if (c.a != 0xff)
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	SDL_Rect sides[4];
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);

	sides[0].x = rect.x;
	sides[0].y = rect.y + strokeWidth;
	sides[0].w = strokeWidth;
	sides[0].h = rect.h - strokeWidth * 2;

	sides[1].x = rect.x + rect.w - strokeWidth;
	sides[1].y = rect.y + strokeWidth;
	sides[1].w = strokeWidth;
	sides[1].h = rect.h - strokeWidth * 2;

	sides[2].x = rect.x;
	sides[2].y = rect.y;
	sides[2].w = rect.w;
	sides[2].h = strokeWidth;

	sides[3].x = rect.x;
	sides[3].y = rect.y + rect.h - strokeWidth;
	sides[3].w = rect.w;
	sides[3].h = strokeWidth;

	SDL_RenderFillRects(renderer, sides, 4);

	if (c.a != 0xff)
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void Graphics::SetClipRect(Rectangle rect)
{
	SDL_Rect clipRect;
	clipRect.x = rect.x;
	clipRect.y = rect.y;
	clipRect.w = rect.w;
	clipRect.h = rect.h;

	SDL_RenderSetClipRect(renderer, &clipRect);
}

void Graphics::ClearClipRect()
{
	SDL_RenderSetClipRect(renderer, NULL);
}

SDL_Surface* Graphics::ClipSurface(SDL_Surface* surface, SDL_Rect* clip)
{
	if (surface == NULL)
		return NULL;

	if (clip == NULL)
		return NULL;

	int newW = std::min(clip->w, std::max(surface->w - clip->x, 0));
	int newH = std::min(clip->h, std::max(surface->h - clip->y, 0));

	if (newW == 0 || newH == 0)
		return NULL;

	SDL_Surface* newSurface = CreateEmptySurface(newW, newH);

	if (newSurface == NULL)
		return NULL;

	SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_NONE);
	DrawSurface(0, 0, surface, newSurface, clip);

	return newSurface;
}

bool Graphics::FadeOutSurface(SDL_Surface* surface, int fadeLength)
{
	if (surface->w < fadeLength)
		return false;

	for (int x = surface->w - 1; x >= surface->w - fadeLength; x--)
	{
		double distanceFromBorder = surface->w - 1 - x;
		double columnAlpha = std::min((distanceFromBorder + 1) / fadeLength, 1.0);
		for (int y = 0; y < surface->h; y++)
		{
			Uint8 r, g, b, a;
			Uint32 px = GetPixel(surface, x, y);
			GetColorComponents(px, surface->format, r, g, b, a);
			double ad = (double)a / (double)255;

			PutPixel(surface, x, y, SDL_MapRGBA(surface->format, r, g, b, (Uint8)((ad * columnAlpha) * 255)));
		}
	}

	return true;
}

SDL_BlendMode Graphics::ApplyBlendMode(SDL_Surface * surface, SDL_BlendMode newBlendMode)
{
	SDL_BlendMode oldBlendMode;
	SDL_GetSurfaceBlendMode(surface, &oldBlendMode);
	SDL_SetSurfaceBlendMode(surface, newBlendMode);

	return oldBlendMode;
}
