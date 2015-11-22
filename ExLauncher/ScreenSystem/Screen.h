#ifndef _SCREEN_H
#define _SCREEN_H

/*********************************************/

#include <SDL.h>
#include <string>
#include "../graphics_utils.h"
#include "../ScreenSystem/InputState.h"
#include "../ViewSystem/View.h"
#include "../DataStore.h"

/*********************************************/

enum ScreenState { TransitionOn, Active, TransitionOff, Hidden };

/*********************************************/

class ScreenManager;

class Screen
{
public:

	Screen();

	virtual ~Screen();

	virtual void Update(bool otherScreenHasFocus, bool coveredByOtherScreen);

	// Does complementary draw work. Calls the Draw method. This handled by the ScreenManager, and should not be done manually.
	void DrawHelper(SDL_Renderer* renderer);

	// Handles input. Calling of this method is handled by the ScreenManager, and should not be done manually.
	virtual void HandleInput(InputState* input) = 0;

	// Initializes the screen. This handled by the ScreenManager, and should not be done manually.
	virtual bool Initialize() = 0;

	// Gets the last error text.
	std::string GetLastError();

	// Gets the state of the screen.
	ScreenState GetScreenState();

	// Exits the screen. It will be automatically be deleted by the ScreenManager.
	void ExitScreen();

	// Returns true if the screen is in the process of exiting (transitioning out)
	bool IsExiting();

	// Returns true if the screen is exiting.
	bool HasExited();

	// Returns true if the screen is a popup screen.
	bool IsPopup();

	// Returns true if the screen is active and can respond to input
	bool IsActive();

	// Returns true if the screen is done transitioning on
	bool TransitionHasFinished();

	// Returns the renderer used for this screen
	SDL_Renderer* GetRenderer();

	// Returns the resource manager used for this screen
	ResourceManager* GetResourceManager();

	// Sets the ScreenManager object. This handled by the ScreenManager, and should not be done manually.
	void SetScreenManager(ScreenManager* screenManager);

	// Used to pass events, for example a view (an item) passing an launch event
	virtual void OnEvent(View* sender, EventType eventType, std::string eventValue, std::vector<std::string> eventArgs = std::vector<std::string>());
private:
	bool isExiting;
	bool exited;

	// Updates the transition. Returns true if still transitioning, and false if done. Direction is either 1 or -1.
	bool UpdateTransition(double time, int direction);
protected:
	// Handles drawing. Calling of this method is handled by the ScreenManager, and should not be done manually.
	virtual void Draw(SDL_Renderer* renderer) = 0;

	double GetTransitionPosition();
	Uint8 GetTransitionAlpha();
	void SetTransitionTime(double transitionOnTime, double transitionOffTime);

	ScreenManager* screenManager;
	DataStore* arguments;
	ScreenState screenstate;
	std::string lastError;
	bool popup;
	double transitionPosition;
	double transitionOnTime;
	double transitionOffTime;
	bool otherScreenHasFocus;
};

/*********************************************/

#endif