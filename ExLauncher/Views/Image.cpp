#include "Image.h"
#include "../ThemeManager.h"
#include "../ScreenSystem/Screen.h"

using namespace std;

Image::Image()
{
	image = NULL;
	path = "";
	altPath = "";
}

Image::~Image()
{
}

bool Image::OnInitialize()
{
	image = context->GetResourceManager()->LoadImage(path, path.c_str());
	if (image == NULL)
	{
		image = context->GetResourceManager()->LoadImage(altPath, altPath.c_str());
		if (image == NULL)
			return false;
	}

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
	view->SetAltPath(altPath);

	return view;
}

void Image::SetPath(string path)
{
	this->path = path;
}

void Image::SetAltPath(string path)
{
	this->altPath = path;
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
	else if (name == "altPath")
	{
		SetAltPath(ThemeManager::ProcessPath(value));
		return true;
	}

	return false;
}

void Image::FillData(map<string, string>& data)
{
	path = FindAndReplace(path, data);
	altPath = FindAndReplace(altPath, data);

	if (IsInitialized())
	{
		image = context->GetResourceManager()->LoadImage(path, path.c_str());
		if (image == NULL)
			image = context->GetResourceManager()->LoadImage(altPath, altPath.c_str());

		if (image != NULL)
			SDL_QueryTexture(image, NULL, NULL, &contentSize.w, &contentSize.h);

		RecalculateLayout();
	}
}
