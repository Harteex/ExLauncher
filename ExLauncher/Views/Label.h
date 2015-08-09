#ifndef _LABEL_H_
#define _LABEL_H_

/*********************************************/

#include "../ViewSystem/View.h"
#include "../structures.h"
#include <vector>
#include <string>
#include "../graphics_utils.h"
#include <SDL_ttf.h>

/*********************************************/

class Label : public View
{
private:
	SDL_Texture* texture;
	std::string text;
	std::string calculatedText;
	int textSize;
	Color textColor;
	std::string font;
	ResourceManager* resourceManager;
	TTF_Font* ttfFont;

	bool RenderText(Uint32 textAreaWidth);
public:
	Label();
	~Label();
	bool OnInitialize();
	void OnDraw(SDL_Renderer* renderer, Position offset);
	void OnLayoutChange();
	View* Copy();

	std::string GetText();
	void SetText(std::string text);
	int GetTextSize();
	void SetTextSize(int textSize);
	Color GetTextColor();
	void SetTextColor(Color textColor);
	std::string GetFont();
	void SetFont(std::string font);
	bool SetProperty(std::string name, std::string value);
	void FillData(std::map<std::string, std::string>& data);
	void CheckIfDataCanBeFilled();
};

/*********************************************/

#endif
