#ifndef _LABEL_H_
#define _LABEL_H_

/*********************************************/

#include "../ViewSystem/View.h"
#include "../structures.h"
#include <vector>
#include <string>
#include "../graphics.h"
#include <SDL_ttf.h>

/*********************************************/

class Label : public View
{
private:
	SDL_Texture* texture;
	std::string text;
	std::string calculatedText;
	ResourceManager* resourceManager;
	SDL_Renderer* renderer;
	TTF_Font* font;

	bool RenderText(Uint32 textAreaWidth);
public:
	Label();
	~Label();
	bool Initialize(ResourceManager* resourceManager, SDL_Renderer* renderer);
	void Update();
	void Draw(SDL_Renderer* renderer);
	void OnLayoutChange();
	View* Copy();

	void SetText(std::string text);
	bool SetProperty(std::string name, std::string value);
	void FillData(std::map<std::string, std::string>& data);
};

/*********************************************/

#endif
