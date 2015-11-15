#ifndef _IMAGE_H_
#define _IMAGE_H_

/*********************************************/

#include "../ViewSystem/View.h"
#include "../structures.h"
#include <vector>
#include <string>
#include "../graphics_utils.h"

/*********************************************/

class Image : public View
{
private:
	SDL_Texture* image;
	std::string path;
	std::string altPath;
public:
	Image();
	~Image();
	bool OnInitialize();
	void OnDraw(SDL_Renderer* renderer, Position offset);
	View* Copy();

	void SetPath(std::string path);
	void SetAltPath(std::string path);
	bool SetProperty(std::string name, std::string value);
	void FillData(std::map<std::string, std::string>& data);
};

/*********************************************/

#endif
