// Adapted from code by Michal Dymel, MIT license
// https://github.com/mdymel/superfastblur
// Which in turn is based on code and algorithms by Ivan Kutskir, MIT license
// http://blog.ivank.net/fastest-gaussian-blur.html

#ifndef _GAUSSIAN_BLUR_H_
#define _GAUSSIAN_BLUR_H_

/*********************************************/

#include <SDL.h>

/*********************************************/

class GaussianBlur
{
public:
	GaussianBlur(SDL_Surface* image);
	~GaussianBlur();

	SDL_Surface* Process(int radial);
	SDL_Surface* ProcessAlphaOnly(int radial);
private:
	int* _alpha;
	int* _red;
	int* _green;
	int* _blue;

	int _width;
	int _height;

	void GaussBlur4(int* source, int* dest, int r);
	int* BoxesForGauss(int sigma, int n);
	void BoxBlur4(int* source, int* dest, int w, int h, int r);
	void BoxBlurH4(int* source, int* dest, int w, int h, int r);
	void BoxBlurT4(int* source, int* dest, int w, int h, int r);
};

/*********************************************/

#endif
