#ifndef _APP_H_
#define _APP_H_

/*********************************************/

#include <SDL.h>
#include <string>
#include <map>

/*********************************************/

class App
{
private:
	std::map<std::string, std::string> data;
public:
	/*std::string name;
	std::string target;
	SDL_Texture* icon;
	std::string description;
	std::string manual;*/

	std::map<std::string, std::string>& GetAllData();
	std::string GetData(std::string key, std::string defaultValue);
	void SetData(std::string key, std::string value);

	App();
	//void HandleInput(InputState* input);
	//bool Initialize();
	//void Update(bool otherScreenHasFocus, bool coveredByOtherScreen);
protected:
	//void Draw(SDL_Renderer* renderer);
	//void OnFormEvent(std::string id, FormEventType eventType);
};

/*********************************************/

#endif
