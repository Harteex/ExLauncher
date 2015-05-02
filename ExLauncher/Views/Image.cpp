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
	this->renderer = renderer;
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

void Image::OnDraw(SDL_Renderer* renderer)
{
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
	Image* view = new Image();

	CopyBase(view);
	view->SetPath(path);

	return view;
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
