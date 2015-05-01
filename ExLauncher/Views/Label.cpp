#include "Label.h"
#include <SDL.h>
#include "../graphics.h"

using namespace std;

Label::Label()
{
	texture = NULL;
	text = "";
	calculatedText = "";
}

Label::~Label()
{
	if (texture != NULL)
		SDL_DestroyTexture(texture);
}

bool Label::Initialize(ResourceManager* resourceManager, SDL_Renderer* renderer)
{
	this->resourceManager = resourceManager;
	this->renderer = renderer;
	font = resourceManager->GetTTFFont("guiFontSmall");

	if (font == NULL)
		return false;

	if (!RenderText(UINT_MAX))
		return false;

	isInitialized = true;
	return true;
}

bool Label::RenderText(Uint32 textAreaWidth)
{
	if (texture != NULL)
		SDL_DestroyTexture(texture);

	texture = NULL;

	SDL_Color color;
	color.r = 0xff;
	color.g = 0xff;
	color.b = 0xff;
	color.a = 0xff;
	SDL_Surface* tempSurface = TTF_RenderText_Blended_Wrapped(font, calculatedText.empty() ? text.c_str() : calculatedText.c_str(), color, textAreaWidth);
	if (tempSurface == NULL)
		return false;

	SDL_Surface* clippedSurface = NULL;

	if (calculatedSize.w > 0 && tempSurface->w > calculatedSize.w)
	{
		SDL_Rect r;
		r.x = 0;
		r.y = 0;
		r.w = calculatedSize.w;
		r.h = tempSurface->h;
		clippedSurface = ClipSurface(tempSurface, &r);

		SDL_FreeSurface(tempSurface);

		// Apply alpha fadeout at end if text doesn't fit completely
		FadeOutSurface(clippedSurface, 8);
	}
	else
	{
		clippedSurface = tempSurface;
	}

	if (clippedSurface == NULL)
		return false;

	texture = SDL_CreateTextureFromSurface(renderer, clippedSurface);
	SDL_FreeSurface(clippedSurface);
	if (texture == NULL)
		return false;

	SDL_QueryTexture(texture, NULL, NULL, &contentSize.w, &contentSize.h);

	return true;
}

void Label::Update()
{
}

void Label::OnDraw(SDL_Renderer* renderer)
{
	if (texture == NULL)
		return;

	SDL_Rect r;
	r.x = absolutePosition.x;
	r.y = absolutePosition.y;
	r.w = contentSize.w;
	r.h = contentSize.h;

	Position gravityOffset = GetGravityOffset(contentSize, calculatedSize, gravity);
	r.x += gravityOffset.x;
	r.y += gravityOffset.y;

	drawTexture(&r, texture, renderer);

	SDL_RenderSetClipRect(renderer, NULL);
}

void Label::OnLayoutChange()
{
	if (size.w == SIZE_MATCH_PARENT || size.h == SIZE_MATCH_PARENT)
		RenderText(UINT_MAX);
}

View* Label::Copy()
{
	Label* view = new Label();

	CopyBase(view);
	view->SetText(text);

	return view;
}

void Label::SetText(string text)
{
	this->text = text;

	if (isInitialized)
	{
		RenderText(UINT_MAX);

		if (size.w == SIZE_WRAP_CONTENT || size.h == SIZE_WRAP_CONTENT)
			RecalculateLayout();
	}
}

bool Label::SetProperty(string name, string value)
{
	bool propertyHandled = View::SetProperty(name, value);

	if (propertyHandled)
		return true;

	if (name == "text")
	{
		SetText(value);
		return true;
	}

	return false;
}

void Label::FillData(map<string, string>& data)
{
	calculatedText = text;
	int searchFrom = 0;

	while (true)
	{
		size_t foundStart = calculatedText.find_first_of('{', searchFrom);
		if (foundStart == string::npos)
			return;

		size_t foundEnd = calculatedText.find_first_of('}', foundStart + 1);
		if (foundEnd == string::npos)
			return;

		try
		{
			string key = calculatedText.substr(foundStart + 1, foundEnd - foundStart - 1);
			string replaceWith = data.at(key);
			int keyLength = foundEnd - foundStart - 1 + 2; // + 2 inc brackets
			int replaceWithLength = replaceWith.length();
			int sizeChange = replaceWithLength - keyLength;

			searchFrom = foundEnd + sizeChange + 1;
			calculatedText.replace(foundStart, keyLength, replaceWith);
		}
		catch (exception& ex)
		{
			searchFrom = foundEnd + 1;
		}
	}

	if (isInitialized)
	{
		RenderText(UINT_MAX);
		RecalculateLayout();
	}
}
