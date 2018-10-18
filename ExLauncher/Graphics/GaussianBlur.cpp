// Adapted from code by Michal Dymel, MIT license
// https://github.com/mdymel/superfastblur
// Which in turn is based on code and algorithms by Ivan Kutskir, MIT license
// http://blog.ivank.net/fastest-gaussian-blur.html

#include "GaussianBlur.h"
#include <SDL.h>
#include <SDL_image.h>
#include <cstring>
#include <cmath>

GaussianBlur::GaussianBlur(SDL_Surface * image)
{
	auto newImage = (SDL_Surface*)SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_ARGB8888, 0);

	_width = newImage->w;
	_height = newImage->h;

	int* source = (int*)newImage->pixels;

	_alpha = new int[_width * _height];
	_red = new int[_width * _height];
	_green = new int[_width * _height];
	_blue = new int[_width * _height];

	for (int i = 0; i < (_width * _height); i++)
	{
		_alpha[i] = (int)((source[i] & 0xff000000) >> 24);
		_red[i] = (source[i] & 0xff0000) >> 16;
		_green[i] = (source[i] & 0x00ff00) >> 8;
		_blue[i] = (source[i] & 0x0000ff);
	}

	SDL_FreeSurface(newImage);
}

GaussianBlur::~GaussianBlur()
{
	if (_alpha != NULL)
		delete[] _alpha;

	if (_red != NULL)
		delete[] _red;

	if (_green != NULL)
		delete[] _green;

	if (_blue != NULL)
		delete[] _blue;
}

SDL_Surface* GaussianBlur::Process(int radial)
{
	auto newAlpha = new int[_width * _height];
	auto newRed = new int[_width * _height];
	auto newGreen = new int[_width * _height];
	auto newBlue = new int[_width * _height];
	auto dest = new int[_width * _height];

	GaussBlur4(_alpha, newAlpha, radial);
	GaussBlur4(_red, newRed, radial);
	GaussBlur4(_green, newGreen, radial);
	GaussBlur4(_blue, newBlue, radial);

	for (int i = 0; i < (_width * _height); i++)
	{
		if (newAlpha[i] > 255) newAlpha[i] = 255;
		if (newRed[i] > 255) newRed[i] = 255;
		if (newGreen[i] > 255) newGreen[i] = 255;
		if (newBlue[i] > 255) newBlue[i] = 255;

		if (newAlpha[i] < 0) newAlpha[i] = 0;
		if (newRed[i] < 0) newRed[i] = 0;
		if (newGreen[i] < 0) newGreen[i] = 0;
		if (newBlue[i] < 0) newBlue[i] = 0;

		dest[i] = (int)((unsigned int)(newAlpha[i] << 24) | (unsigned int)(newRed[i] << 16) | (unsigned int)(newGreen[i] << 8) | (unsigned int)newBlue[i]);
	}

	delete[] newAlpha;
	delete[] newRed;
	delete[] newGreen;
	delete[] newBlue;

	auto image = SDL_CreateRGBSurfaceWithFormat(0, _width, _height, 32, SDL_PIXELFORMAT_ARGB8888);
	memcpy(image->pixels, dest, _width * _height * 4);

	delete[] dest;
	
	return image;
}

void GaussianBlur::GaussBlur4(int* source, int* dest, int r)
{
	auto bxs = BoxesForGauss(r, 3);

	BoxBlur4(source, dest, _width, _height, (bxs[0] - 1) / 2);
	BoxBlur4(dest, source, _width, _height, (bxs[1] - 1) / 2);
	BoxBlur4(source, dest, _width, _height, (bxs[2] - 1) / 2);

	delete[] bxs;
}

int* GaussianBlur::BoxesForGauss(int sigma, int n)
{
	double wIdeal = sqrt((12 * sigma * sigma / n) + 1);
	int wl = (int)floor(wIdeal);
	if (wl % 2 == 0) wl--;
	int wu = wl + 2;

	double mIdeal = (double)(12 * sigma * sigma - n * wl * wl - 4 * n * wl - 3 * n) / (-4 * wl - 4);
	double m = round(mIdeal);

	auto sizes = new int[n];
	for (int i = 0; i < n; i++)
		sizes[i] = (i < m ? wl : wu);

	return sizes;
}

void GaussianBlur::BoxBlur4(int* source, int* dest, int w, int h, int r)
{
	int length = w * h;
	for (int i = 0; i < length; i++)
		dest[i] = source[i];

	BoxBlurH4(dest, source, w, h, r);
	BoxBlurT4(source, dest, w, h, r);
}

void GaussianBlur::BoxBlurH4(int* source, int* dest, int w, int h, int r)
{
	double iar = (double)1 / (r + r + 1);
	for (int i = 0; i < h; i++)
	{
		int ti = i * w;
		int li = ti;
		int ri = ti + r;
		int fv = source[ti];
		int lv = source[ti + w - 1];
		int val = (r + 1) * fv;

		for (int j = 0; j < r; j++)
			val += source[ti + j];

		for (int j = 0; j <= r; j++)
		{
			val += source[ri++] - fv;
			dest[ti++] = (int)round(val * iar);
		}

		for (int j = r + 1; j < w - r; j++)
		{
			val += source[ri++] - dest[li++];
			dest[ti++] = (int)round(val * iar);
		}

		for (int j = w - r; j < w; j++)
		{
			val += lv - source[li++];
			dest[ti++] = (int)round(val * iar);
		}
	}
}

void GaussianBlur::BoxBlurT4(int* source, int* dest, int w, int h, int r)
{
	double iar = (double)1 / (r + r + 1);
	for (int i = 0; i < w; i++)
	{
		int ti = i;
		int li = ti;
		int ri = ti + r * w;
		int fv = source[ti];
		int lv = source[ti + w * (h - 1)];
		int val = (r + 1) * fv;

		for (int j = 0; j < r; j++)
			val += source[ti + j * w];

		for (int j = 0; j <= r; j++)
		{
			val += source[ri] - fv;
			dest[ti] = (int)round(val * iar);
			ri += w;
			ti += w;
		}

		for (int j = r + 1; j < h - r; j++)
		{
			val += source[ri] - source[li];
			dest[ti] = (int)round(val * iar);
			li += w;
			ri += w;
			ti += w;
		}

		for (int j = h - r; j < h; j++)
		{
			val += lv - source[li];
			dest[ti] = (int)round(val * iar);
			li += w;
			ti += w;
		}
	}
}
