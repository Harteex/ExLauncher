#include "graphics_utils.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <SDL_image.h>
#include "utils.h"
#include <map>
#include "global.h"
#include <algorithm>

using namespace std;

const double PI = 3.14159265358979323846;
map<int, double*> gaussianKernels;

// This function comes from the SDL documentation
Uint32 getPixel(SDL_Surface *surface, int x, int y)
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
        return 0;       // shouldn't happen, but avoids warnings
    }
}

// This function comes from the SDL documentation
void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
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

void dumpSurface(char* filename, SDL_Surface* s)
{
	FILE* f = fopen(filename, "w");
	for (int y = 0; y < s->h; y++)
	{
		for (int x = 0; x < s->w; x++)
		{
			Uint32 px = getPixel(s, x, y);
			fwrite(&px, sizeof(Uint32), 1, f);
		}
	}
	fclose(f);
}

void getColorComponents(Uint32 color, SDL_PixelFormat* format, Uint8& red, Uint8& green, Uint8& blue, Uint8& alpha)
{
	Uint32 temp;

	/* Get Red component */
	temp = color & format->Rmask;  /* Isolate red component */
	temp = temp >> format->Rshift; /* Shift it down to 8-bit */
	temp = temp << format->Rloss;  /* Expand to a full 8-bit number */
	red = (Uint8)temp;

	/* Get Green component */
	temp = color & format->Gmask;  /* Isolate green component */
	temp = temp >> format->Gshift; /* Shift it down to 8-bit */
	temp = temp << format->Gloss;  /* Expand to a full 8-bit number */
	green = (Uint8)temp;

	/* Get Blue component */
	temp = color & format->Bmask;  /* Isolate blue component */
	temp = temp >> format->Bshift; /* Shift it down to 8-bit */
	temp = temp << format->Bloss;  /* Expand to a full 8-bit number */
	blue = (Uint8)temp;

	/* Get Alpha component */
	temp = color & format->Amask;  /* Isolate alpha component */
	temp = temp >> format->Ashift; /* Shift it down to 8-bit */
	temp = temp << format->Aloss;  /* Expand to a full 8-bit number */
	alpha = (Uint8)temp;
}

SDL_Texture* createEmptyTexture(SDL_Renderer* renderer, int width, int height)
{
	SDL_Texture* texture;
	
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, width, height);
	return texture;
}

SDL_Surface* createEmptySurface(int width, int height)
{
	SDL_Surface *surface;
    Uint32 rmask, gmask, bmask, amask;

    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
       on the endianness (byte order) of the machine */
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

SDL_Surface* loadSurface(const char* filename)
{
	if (filename == NULL)
		return NULL;

	return IMG_Load(filename);
}

/*SDL_Texture* loadImageFromBuffer(void* buffer, SDL_Renderer* renderer)
{
	if (buffer == NULL)
		return NULL;

	SDL_Surface* temp;
	if (strncmp((char*)buffer, "HIF", 3) == 0)
	{
		temp = hif_load_from_buffer((uint8_t*)buffer);
	}
	else
	{
		temp = NULL; //SDL_LoadBMP_RW(
	}

	char* test = (char*)buffer;
	
	if (temp == NULL)
		return NULL;

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, temp);

	SDL_FreeSurface(temp);

	return texture;
}*/

SDL_Texture* loadImage(const char* filename, SDL_Renderer* renderer)
{
	SDL_Surface* temp = loadSurface(filename);
    
	if (temp == NULL)
		return NULL;

	int colorkey = SDL_MapRGB(temp->format, 0xff, 0x00, 0xff);
	SDL_SetColorKey(temp, SDL_TRUE, colorkey);

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, temp);

	SDL_FreeSurface(temp);

	return texture;
}

SDL_Texture* loadImageFromMemory(void* buffer, int size, SDL_Renderer* renderer)
{
	SDL_RWops* sdlRw = SDL_RWFromMem(buffer, size);
	SDL_Surface *image = IMG_Load_RW(sdlRw, 1);

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);

	SDL_FreeSurface(image);

	return texture;
}

void drawSurface(int x, int y, SDL_Surface* source, SDL_Surface* dest)
{
	drawSurface(x, y, source, dest, NULL);
}

void drawSurface(int x, int y, SDL_Surface* source, SDL_Surface* dest, SDL_Rect* clip)
{
	SDL_Rect coords;
	coords.x = x;
	coords.y = y;

	SDL_BlitSurface(source, clip, dest, &coords);
}

void drawTexture(int x, int y, SDL_Texture* texture, SDL_Renderer* renderer)
{
	drawTexture(x, y, texture, renderer, TOP|LEFT, NULL);
}

void drawTexture(SDL_Rect* dstRect, SDL_Texture* texture, SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, texture, NULL, dstRect);
}

void drawTexture(int x, int y, SDL_Texture* texture, SDL_Renderer* renderer, int anchor)
{
	drawTexture(x, y, texture, renderer, anchor, NULL);
}

void drawTexture(int x, int y, SDL_Texture* texture, SDL_Renderer* renderer, int anchor, SDL_Rect* clip)
{
	drawTexture(x, y, texture, renderer, anchor, clip, 0.0, 1.0, false);
}

void drawTexture(int x, int y, SDL_Texture* texture, SDL_Renderer* renderer, int anchor, SDL_Rect* clip, double angle, double scale, bool scaleAdjustPosition)
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

void drawTextureCenter(int y, SDL_Texture* texture, SDL_Renderer* renderer, int areaWidth, int offsetX)
{
	int w;
	SDL_QueryTexture(texture, NULL, NULL, &w, NULL);

	int x = (areaWidth / 2) - (w / 2) + offsetX;

	drawTexture(x, y, texture, renderer);
}

bool setAllTransparentPixelsToColor(SDL_Surface* surface, Uint32 color)
{
	Uint8 r, g, b, a;

	if (SDL_MUSTLOCK(surface))
	{
		if (SDL_LockSurface(surface) != 0)
		{
			return false;
		}
	}

	// Make sure alpha is set to zero (transparent) in the color
	getColorComponents(color, surface->format, r, g, b, a);
	color = SDL_MapRGBA(surface->format, r, g, b, 0x00);

	for (int y = 0; y < surface->h; y++)
	{
		for (int x = 0; x < surface->w; x++)
		{
			Uint32 curColor = getPixel(surface, x, y);
			getColorComponents(curColor, surface->format, r, g, b, a);
			if (a == 0x00)
			{
				putPixel(surface, x, y, color);
			}
		}
	}

	if (SDL_MUSTLOCK(surface))
		SDL_UnlockSurface(surface);
	
	return true;
}

SDL_Surface* extendSurface(SDL_Surface* srcSurface, int paddingTopBottom, int paddingLeftRight) // FIXME this is bugged in my VM
{
	SDL_Surface* newSurface = SDL_CreateRGBSurface(0, srcSurface->w + paddingLeftRight * 2, srcSurface->h + paddingTopBottom * 2, srcSurface->format->BitsPerPixel,
		srcSurface->format->Rmask, srcSurface->format->Gmask, srcSurface->format->Bmask, srcSurface->format->Amask);

	if (newSurface == NULL)
		return NULL;

	drawSurface(paddingLeftRight, paddingTopBottom, srcSurface, newSurface);

	return newSurface;
}

double* getGaussianKernel(int radius)
{
	printf("f start\n");
	double* kernel = gaussianKernels[radius];
	if (kernel != NULL)
		return kernel;

	printf("f create new!\n");

	int size = radius * 2 + 1;
	kernel = new double[size];

    // set standard deviation to 1.0
    //double sigma = 1.0;
	// http://stackoverflow.com/questions/17841098/gaussian-blur-standard-deviation-radius-and-kernel-size
	double sigma = 0.3 * ((double)size / 2 - 1) + 0.8; // FIXME TEST THIS!!! look at generated kernels... make sure they add up to like 1
    double r, s = 2.0 * sigma * sigma;
 
    // sum is for normalization
    double sum = 0.0;
 
    // generate n kernel
    for (int x = -radius; x <= radius; x++)
    {
        r = sqrt((double)x*x);
		double cur = (exp(-(r*r)/s))/(PI * s);
        kernel[x + radius] = cur;
        sum += cur;
    }
 
    // normalize the Kernel
    for(int i = 0; i < size; ++i)
        kernel[i] /= sum;

	gaussianKernels[radius] = kernel;

	return kernel;
}

SDL_Surface* blurSurface(SDL_Surface* srcSurface, int radius)
{
	if (srcSurface == NULL)
		return NULL;

	SDL_Surface* dstSurface = SDL_CreateRGBSurface(0, srcSurface->w, srcSurface->h, srcSurface->format->BitsPerPixel,
		srcSurface->format->Rmask, srcSurface->format->Gmask, srcSurface->format->Bmask, srcSurface->format->Amask);

	SDL_Surface* tmpSurface = SDL_CreateRGBSurface(0, srcSurface->w, srcSurface->h, srcSurface->format->BitsPerPixel,
		srcSurface->format->Rmask, srcSurface->format->Gmask, srcSurface->format->Bmask, srcSurface->format->Amask);

	if (dstSurface == NULL)
		return NULL;

	if (tmpSurface == NULL)
	{
		SDL_FreeSurface(dstSurface);
		return NULL;
	}

	if (SDL_MUSTLOCK(srcSurface))
	{
		if (SDL_LockSurface(srcSurface) != 0)
		{
			SDL_FreeSurface(dstSurface);
			SDL_FreeSurface(tmpSurface);
			return NULL;
		}
	}

	if (SDL_MUSTLOCK(dstSurface))
	{
		if (SDL_LockSurface(dstSurface) != 0)
		{
			if (SDL_MUSTLOCK(srcSurface))
				SDL_UnlockSurface(srcSurface);

			SDL_FreeSurface(dstSurface);
			SDL_FreeSurface(tmpSurface);
			return NULL;
		}
	}

	if (SDL_MUSTLOCK(tmpSurface))
	{
		if (SDL_LockSurface(tmpSurface) != 0)
		{
			if (SDL_MUSTLOCK(srcSurface))
				SDL_UnlockSurface(srcSurface);

			if (SDL_MUSTLOCK(dstSurface))
				SDL_UnlockSurface(dstSurface);

			SDL_FreeSurface(dstSurface);
			SDL_FreeSurface(tmpSurface);
			return NULL;
		}
	}

	double* coeffs = getGaussianKernel(radius);

	for (int y = 0; y < srcSurface->h; y++)
	{
		for (int x = 0; x < srcSurface->w; x++)
		{
			double sumR = 0, sumG = 0, sumB = 0, sumA = 0;
			for (int i = -radius; i <= radius; i++)
			{
				int ty = clip(y + i, 0, srcSurface->h - 1);
				Uint32 curPixel = getPixel(srcSurface, x, ty);

				Uint8 tempR, tempG, tempB, tempA;
				getColorComponents(curPixel, srcSurface->format, tempR, tempG, tempB, tempA);

				sumR += ((double)tempR * coeffs[i + radius]);
				sumG += ((double)tempG * coeffs[i + radius]);
				sumB += ((double)tempB * coeffs[i + radius]);
				sumA += ((double)tempA * coeffs[i + radius]);
			}

			Uint32 newPixel = SDL_MapRGBA(tmpSurface->format, sumR, sumG, sumB, sumA);
			putPixel(tmpSurface, x, y, newPixel);
		}
	}

	for (int y = 0; y < srcSurface->h; y++)
	{
		for (int x = 0; x < srcSurface->w; x++)
		{
			double sumR = 0, sumG = 0, sumB = 0, sumA = 0;
			for (int i = -radius; i <= radius; i++)
			{
				int tx = clip(x + i, 0, srcSurface->w - 1);
				Uint32 curPixel = getPixel(tmpSurface, tx, y);

				Uint8 tempR, tempG, tempB, tempA;
				getColorComponents(curPixel, srcSurface->format, tempR, tempG, tempB, tempA);

				sumR += ((double)tempR * coeffs[i + radius]);
				sumG += ((double)tempG * coeffs[i + radius]);
				sumB += ((double)tempB * coeffs[i + radius]);
				sumA += ((double)tempA * coeffs[i + radius]);
			}

			Uint32 newPixel = SDL_MapRGBA(dstSurface->format, sumR, sumG, sumB, sumA);
			putPixel(dstSurface, x, y, newPixel);
		}
	}

	if (SDL_MUSTLOCK(srcSurface))
		SDL_UnlockSurface(srcSurface);

	if (SDL_MUSTLOCK(dstSurface))
		SDL_UnlockSurface(dstSurface);

	if (SDL_MUSTLOCK(tmpSurface))
		SDL_UnlockSurface(tmpSurface);

	SDL_FreeSurface(tmpSurface);

	return dstSurface;
}

// TEST!!
SDL_Surface* CreateGradientSurface(int width, int height, SDL_Color fromColor, SDL_Color toColor)
{
	SDL_Surface* surface = createEmptySurface(width, height);
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

SDL_Texture* CreateGradientTexture(int width, int height, Uint32 fromColor, Uint32 toColor, SDL_Renderer* renderer)
{
	//SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, width, height);
	return NULL;
}

SDL_Surface* ClipSurface(SDL_Surface* surface, SDL_Rect* clip)
{
	if (surface == NULL)
		return NULL;

	if (clip == NULL)
		return NULL;

	int newW = min(clip->w, max(surface->w - clip->x, 0));
	int newH = min(clip->h, max(surface->h - clip->y, 0));

	if (newW == 0 || newH == 0)
		return NULL;

	SDL_Surface* newSurface = createEmptySurface(newW, newH);

	if (newSurface == NULL)
		return NULL;

	SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_NONE);
	drawSurface(0, 0, surface, newSurface, clip);

	return newSurface;
}

bool FadeOutSurface(SDL_Surface* surface, int fadeLength)
{
	if (surface->w < fadeLength)
		return false;

	for (int x = surface->w - 1; x >= surface->w - fadeLength; x--)
	{
		double distanceFromBorder = surface->w - 1 - x;
		double columnAlpha = min((distanceFromBorder + 1) / fadeLength, 1.0);
		for (int y = 0; y < surface->h; y++)
		{
			Uint8 r, g, b, a;
			Uint32 px = getPixel(surface, x, y);
			getColorComponents(px, surface->format, r, g, b, a);
			double ad = (double)a / (double)255;

			putPixel(surface, x, y, SDL_MapRGBA(surface->format, r, g, b, (Uint8)((ad * columnAlpha) * 255)));
		}
	}

	return true;
}

void DrawRect(SDL_Surface* dst, SDL_Rect* dstrect, Uint32 color)
{
	DrawRect(dst, dstrect, color, 1);
}

void DrawRect(SDL_Surface* dst, SDL_Rect* dstrect, Uint32 color, int thickness)
{
	SDL_Rect r;
	r.x = dstrect->x;
	r.y = dstrect->y;
	r.w = dstrect->w;
	r.h = thickness;
	SDL_FillRect(dst, &r, color);

	r.y = dstrect->y + dstrect->h - thickness;
	SDL_FillRect(dst, &r, color);

	r.y = dstrect->y;
	r.w = thickness;
	r.h = dstrect->h;
	SDL_FillRect(dst, &r, color);

	r.x = dstrect->x + dstrect->w - thickness;
	SDL_FillRect(dst, &r, color);
}

SDL_Texture* drawText(const char* text, Uint8 r, Uint8 g, Uint8 b, TTF_Font* font, SDL_Renderer* renderer, Uint32 maxWidth)
{
	SDL_Color fontColor;
	fontColor.r = r;
	fontColor.g = g;
	fontColor.b = b;
	fontColor.a = 0xff;
	SDL_Surface* surfaceText = TTF_RenderText_Blended_Wrapped(font, text, fontColor, maxWidth);
	if (surfaceText == NULL)
		return NULL;

	SDL_Texture* textImage = SDL_CreateTextureFromSurface(renderer, surfaceText);
	SDL_FreeSurface(surfaceText);
	if (textImage == NULL)
		return NULL;

	return textImage;
}

SDL_Surface* drawTextSurface(const char* text, Uint8 r, Uint8 g, Uint8 b, TTF_Font* font)
{
	SDL_Color fontColor;
	fontColor.r = r;
	fontColor.g = g;
	fontColor.b = b;
	fontColor.a = 0xff;
	SDL_Surface* surfaceText = TTF_RenderText_Blended(font, text, fontColor);

	return surfaceText;
}

Rectangle ClipRect(Rectangle clip, Rectangle origClip)
{
	int newX, newY, newX2, newY2;
	newX = clip.x < origClip.x ? origClip.x : clip.x;
	newY = clip.y < origClip.y ? origClip.y : clip.y;
	newX2 = (clip.x + clip.w) >(origClip.x + origClip.w) ? (origClip.x + origClip.w) : (clip.x + clip.w);
	newY2 = (clip.y + clip.h) >(origClip.y + origClip.h) ? (origClip.y + origClip.h) : (clip.y + clip.h);

	int newW, newH;
	newW = max(newX2 - newX, 0);
	newH = max(newY2 - newY, 0);

	return Rectangle(newX, newY, newW, newH);
}

map<string, Color> colorMap;

void InitColorMap()
{
	if (!colorMap.empty())
		return;

	colorMap["aliceblue"] = Color(0xfff0f8ff);
	colorMap["antiquewhite"] = Color(0xfffaebd7);
	colorMap["aqua"] = Color(0xff00ffff);
	colorMap["aquamarine"] = Color(0xff7fffd4);
	colorMap["azure"] = Color(0xfff0ffff);
	colorMap["beige"] = Color(0xfff5f5dc);
	colorMap["bisque"] = Color(0xffffe4c4);
	colorMap["black"] = Color(0xff000000);
	colorMap["blanchedalmond"] = Color(0xffffebcd);
	colorMap["blue"] = Color(0xff0000ff);
	colorMap["blueviolet"] = Color(0xff8a2be2);
	colorMap["brown"] = Color(0xffa52a2a);
	colorMap["burlywood"] = Color(0xffdeb887);
	colorMap["cadetblue"] = Color(0xff5f9ea0);
	colorMap["chartreuse"] = Color(0xff7fff00);
	colorMap["chocolate"] = Color(0xffd2691e);
	colorMap["coral"] = Color(0xffff7f50);
	colorMap["cornflowerblue"] = Color(0xff6495ed);
	colorMap["cornsilk"] = Color(0xfffff8dc);
	colorMap["crimson"] = Color(0xffdc143c);
	colorMap["cyan"] = Color(0xff00ffff);
	colorMap["darkblue"] = Color(0xff00008b);
	colorMap["darkcyan"] = Color(0xff008b8b);
	colorMap["darkgoldenrod"] = Color(0xffb8860b);
	colorMap["darkgray"] = Color(0xffa9a9a9);
	colorMap["darkgrey"] = Color(0xffa9a9a9);
	colorMap["darkgreen"] = Color(0xff006400);
	colorMap["darkkhaki"] = Color(0xffbdb76b);
	colorMap["darkmagenta"] = Color(0xff8b008b);
	colorMap["darkolivegreen"] = Color(0xff556b2f);
	colorMap["darkorange"] = Color(0xffff8c00);
	colorMap["darkorchid"] = Color(0xff9932cc);
	colorMap["darkred"] = Color(0xff8b0000);
	colorMap["darksalmon"] = Color(0xffe9967a);
	colorMap["darkseagreen"] = Color(0xff8fbc8f);
	colorMap["darkslateblue"] = Color(0xff483d8b);
	colorMap["darkslategray"] = Color(0xff2f4f4f);
	colorMap["darkslategrey"] = Color(0xff2f4f4f);
	colorMap["darkturquoise"] = Color(0xff00ced1);
	colorMap["darkviolet"] = Color(0xff9400d3);
	colorMap["deeppink"] = Color(0xffff1493);
	colorMap["deepskyblue"] = Color(0xff00bfff);
	colorMap["dimgray"] = Color(0xff696969);
	colorMap["dimgrey"] = Color(0xff696969);
	colorMap["dodgerblue"] = Color(0xff1e90ff);
	colorMap["firebrick"] = Color(0xffb22222);
	colorMap["floralwhite"] = Color(0xfffffaf0);
	colorMap["forestgreen"] = Color(0xff228b22);
	colorMap["fuchsia"] = Color(0xffff00ff);
	colorMap["gainsboro"] = Color(0xffdcdcdc);
	colorMap["ghostwhite"] = Color(0xfff8f8ff);
	colorMap["gold"] = Color(0xffffd700);
	colorMap["goldenrod"] = Color(0xffdaa520);
	colorMap["gray"] = Color(0xff808080);
	colorMap["grey"] = Color(0xff808080);
	colorMap["green"] = Color(0xff008000);
	colorMap["greenyellow"] = Color(0xffadff2f);
	colorMap["honeydew"] = Color(0xfff0fff0);
	colorMap["hotpink"] = Color(0xffff69b4);
	colorMap["indianred"] = Color(0xffcd5c5c);
	colorMap["indigo"] = Color(0xff4b0082);
	colorMap["ivory"] = Color(0xfffffff0);
	colorMap["khaki"] = Color(0xfff0e68c);
	colorMap["lavender"] = Color(0xffe6e6fa);
	colorMap["lavenderblush"] = Color(0xfffff0f5);
	colorMap["lawngreen"] = Color(0xff7cfc00);
	colorMap["lemonchiffon"] = Color(0xfffffacd);
	colorMap["lightblue"] = Color(0xffadd8e6);
	colorMap["lightcoral"] = Color(0xfff08080);
	colorMap["lightcyan"] = Color(0xffe0ffff);
	colorMap["lightgoldenrodyellow"] = Color(0xfffafad2);
	colorMap["lightgray"] = Color(0xffd3d3d3);
	colorMap["lightgrey"] = Color(0xffd3d3d3);
	colorMap["lightgreen"] = Color(0xff90ee90);
	colorMap["lightpink"] = Color(0xffffb6c1);
	colorMap["lightsalmon"] = Color(0xffffa07a);
	colorMap["lightseagreen"] = Color(0xff20b2aa);
	colorMap["lightskyblue"] = Color(0xff87cefa);
	colorMap["lightslategray"] = Color(0xff778899);
	colorMap["lightslategrey"] = Color(0xff778899);
	colorMap["lightsteelblue"] = Color(0xffb0c4de);
	colorMap["lightyellow"] = Color(0xffffffe0);
	colorMap["lime"] = Color(0xff00ff00);
	colorMap["limegreen"] = Color(0xff32cd32);
	colorMap["linen"] = Color(0xfffaf0e6);
	colorMap["magenta"] = Color(0xffff00ff);
	colorMap["maroon"] = Color(0xff800000);
	colorMap["mediumaquamarine"] = Color(0xff66cdaa);
	colorMap["mediumblue"] = Color(0xff0000cd);
	colorMap["mediumorchid"] = Color(0xffba55d3);
	colorMap["mediumpurple"] = Color(0xff9370db);
	colorMap["mediumseagreen"] = Color(0xff3cb371);
	colorMap["mediumslateblue"] = Color(0xff7b68ee);
	colorMap["mediumspringgreen"] = Color(0xff00fa9a);
	colorMap["mediumturquoise"] = Color(0xff48d1cc);
	colorMap["mediumvioletred"] = Color(0xffc71585);
	colorMap["midnightblue"] = Color(0xff191970);
	colorMap["mintcream"] = Color(0xfff5fffa);
	colorMap["mistyrose"] = Color(0xffffe4e1);
	colorMap["moccasin"] = Color(0xffffe4b5);
	colorMap["navajowhite"] = Color(0xffffdead);
	colorMap["navy"] = Color(0xff000080);
	colorMap["oldlace"] = Color(0xfffdf5e6);
	colorMap["olive"] = Color(0xff808000);
	colorMap["olivedrab"] = Color(0xff6b8e23);
	colorMap["orange"] = Color(0xffffa500);
	colorMap["orangered"] = Color(0xffff4500);
	colorMap["orchid"] = Color(0xffda70d6);
	colorMap["palegoldenrod"] = Color(0xffeee8aa);
	colorMap["palegreen"] = Color(0xff98fb98);
	colorMap["paleturquoise"] = Color(0xffafeeee);
	colorMap["palevioletred"] = Color(0xffdb7093);
	colorMap["papayawhip"] = Color(0xffffefd5);
	colorMap["peachpuff"] = Color(0xffffdab9);
	colorMap["peru"] = Color(0xffcd853f);
	colorMap["pink"] = Color(0xffffc0cb);
	colorMap["plum"] = Color(0xffdda0dd);
	colorMap["powderblue"] = Color(0xffb0e0e6);
	colorMap["purple"] = Color(0xff800080);
	colorMap["red"] = Color(0xffff0000);
	colorMap["rosybrown"] = Color(0xffbc8f8f);
	colorMap["royalblue"] = Color(0xff4169e1);
	colorMap["saddlebrown"] = Color(0xff8b4513);
	colorMap["salmon"] = Color(0xfffa8072);
	colorMap["sandybrown"] = Color(0xfff4a460);
	colorMap["seagreen"] = Color(0xff2e8b57);
	colorMap["seashell"] = Color(0xfffff5ee);
	colorMap["sienna"] = Color(0xffa0522d);
	colorMap["silver"] = Color(0xffc0c0c0);
	colorMap["skyblue"] = Color(0xff87ceeb);
	colorMap["slateblue"] = Color(0xff6a5acd);
	colorMap["slategray"] = Color(0xff708090);
	colorMap["slategrey"] = Color(0xff708090);
	colorMap["snow"] = Color(0xfffffafa);
	colorMap["springgreen"] = Color(0xff00ff7f);
	colorMap["steelblue"] = Color(0xff4682b4);
	colorMap["tan"] = Color(0xffd2b48c);
	colorMap["teal"] = Color(0xff008080);
	colorMap["thistle"] = Color(0xffd8bfd8);
	colorMap["tomato"] = Color(0xffff6347);
	colorMap["turquoise"] = Color(0xff40e0d0);
	colorMap["violet"] = Color(0xffee82ee);
	colorMap["wheat"] = Color(0xfff5deb3);
	colorMap["white"] = Color(0xffffffff);
	colorMap["whitesmoke"] = Color(0xfff5f5f5);
	colorMap["yellow"] = Color(0xffffff00);
	colorMap["yellowgreen"] = Color(0xff9acd32);
}

Color ParseColor(string colorStr)
{
	if (colorStr[0] == '#')
	{
		if (colorStr.length() == 7 || colorStr.length() == 9)
		{
			unsigned int color = strtoul(colorStr.substr(1).c_str(), NULL, 16);

			if (colorStr.length() == 7)
				color += 0xff000000;

			return Color(color);
		}
	}
	else
	{
		InitColorMap();

		auto knownColorResult = colorMap.find(colorStr);
		if (knownColorResult != colorMap.end())
			return knownColorResult->second;
	}

	return Color();
}
