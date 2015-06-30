#include "Image.h"
#include "../ThemeManager.h"
#include "../ScreenSystem/Screen.h"

using namespace std;

Image::Image()
{
	image = NULL;
	path = "";
}

Image::~Image()
{
}

bool Image::OnInitialize()
{
	image = context->GetResourceManager()->LoadImage(path, path.c_str());
	if (image == NULL)
		return false;

	SDL_QueryTexture(image, NULL, NULL, &contentSize.w, &contentSize.h);
	
	return true;
}

void Image::OnDraw(SDL_Renderer* renderer, Position offset)
{
	if (image == NULL)
		return;

	SDL_Rect r;
	r.x = absolutePosition.x + offset.x;
	r.y = absolutePosition.y + offset.y;
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
