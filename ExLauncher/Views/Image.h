#ifndef _IMAGE_H_
#define _IMAGE_H_

/*********************************************/

#include "../ViewSystem/View.h"
#include "../structures.h"
#include <vector>
#include <string>
#include "../graphics.h"

/*********************************************/

class Image : public View
{
private:
	SDL_Texture* image;
	std::string path;
public:
	Image();
	~Image();
	bool Initialize(ResourceManager* resourceManager, SDL_Renderer* renderer);
	void Update();
	void Draw(SDL_Renderer* renderer);
	View* Copy();

	void SetPath(std::string path);
	bool SetProperty(std::string name, std::string value);
};

/*********************************************/

#endif
