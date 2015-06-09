#include "Label.h"
#include <SDL.h>
#include "../graphics_utils.h"
#include <sstream>

using namespace std;

Label::Label()
{
	texture = NULL;
	text = "";
	calculatedText = "";
	textSize = 12;
	textColor = Color(0xff, 0xff, 0xff, 0xff);
	font = "semibold";
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
	ttfFont = resourceManager->GetTTFFont(font, textSize);

	if (ttfFont == NULL)
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
	SDL_Surface* tempSurface = TTF_RenderText_Blended_Wrapped(ttfFont, calculatedText.empty() ? text.c_str() : calculatedText.c_str(), color, textAreaWidth);
	if (tempSurface == NULL)
		return false;

	SDL_Surface* clippedSurface = NULL;

	int areaWidth = size.w;
	if (size.w < 0)
		areaWidth = calculatedSize.w;

	if (areaWidth > 0 && tempSurface->w > areaWidth)
	{
		SDL_Rect r;
		r.x = 0;
		r.y = 0;
		r.w = areaWidth;
		r.h = tempSurface->h;
		clippedSurface = ClipSurface(tempSurface, &r);

		SDL_FreeSurface(tempSurface);

		// Apply alpha fadeout at end if text doesn't fit completely
		FadeOutSurface(clippedSurface, 16);
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

void Label::OnDraw(SDL_Renderer* renderer, Position offset)
{
	if (texture == NULL)
		return;

	SDL_Rect r;
	r.x = absolutePosition.x + offset.x;
	r.y = absolutePosition.y + offset.y;
	r.w = contentSize.w;
	r.h = contentSize.h;

	Position gravityOffset = GetGravityOffset(contentSize, calculatedSize, gravity);
	r.x += gravityOffset.x;
	r.y += gravityOffset.y;

	SDL_SetTextureColorMod(texture, textColor.r, textColor.g, textColor.b);

	drawTexture(&r, texture, renderer);
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
	view->SetTextSize(textSize);
	view->SetTextColor(textColor);
	view->SetFont(font);

	return view;
}

string Label::GetText()
{
	return text;
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

int Label::GetTextSize()
{
	return textSize;
}

void Label::SetTextSize(int textSize)
{
	this->textSize = textSize;
}

Color Label::GetTextColor()
{
	return textColor;
}

void Label::SetTextColor(Color textColor)
{
	this->textColor = textColor;
}

string Label::GetFont()
{
	return font;
}

void Label::SetFont(string font)
{
	this->font = font;
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
	else if (name == "textSize")
	{
		textSize = atoi(value.c_str());

		if (textSize <= 0)
			throw runtime_error("invalid textSize value");

		return true;
	}
	else if (name == "textColor")
	{
		if (value.length() != 7 || value[0] != '#')
			throw runtime_error("invalid textColor value");

		istringstream(value.substr(1, 2)) >> std::hex >> textColor.r;
		istringstream(value.substr(3, 2)) >> std::hex >> textColor.g;
		istringstream(value.substr(5, 2)) >> std::hex >> textColor.b;

		return true;
	}
	else if (name == "font")
	{
		font = value;
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
			int keyLength = foundEnd - foundStart - 1 + 2; // + 2 including brackets
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
