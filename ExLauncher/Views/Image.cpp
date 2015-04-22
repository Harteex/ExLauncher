#include "Image.h"
#include "../ThemeManager.h"

using namespace std;

Image::Image()
{
	image = NULL;
	path = "";
}

Image::~Image()
{
}

bool Image::Initialize(ResourceManager* resourceManager, SDL_Renderer* renderer)
{
	image = resourceManager->LoadImage(path, path.c_str());
	if (image == NULL)
		return false;

	SDL_QueryTexture(image, NULL, NULL, &contentSize.w, &contentSize.h);
	
	isInitialized = true;
	return true;
}

void Image::Update()
{
}

void Image::Draw(SDL_Renderer* renderer)
{
	if (calculatedSize.w == 0 || calculatedSize.h == 0)
		return;

	if (image == NULL)
		return;

	SDL_Rect r;
	r.x = absolutePosition.x;
	r.y = absolutePosition.y;
	r.w = calculatedSize.w;
	r.h = calculatedSize.h;
	drawTexture(&r, image, renderer);
}

View* Image::Copy()
{
	Image* image = new Image();

	image->SetSize(size);
	image->SetRelativePosition(relativePosition);
	image->SetPath(path);

	return image;
}

void Image::SetPath(string path)
{
	this->path = path;
}

bool Image::SetProperty(string name, string value)
{
	bool propertyHandled = View::SetProperty(name, value);

	if (propertyHandled)
		return true;

	if (name == "path")
	{
		SetPath(ThemeManager::ProcessPath(value));
		return true;
	}

	return false;
}
