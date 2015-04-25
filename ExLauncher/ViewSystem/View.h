#ifndef _VIEW_H_
#define _VIEW_H_

/*********************************************/

#include <SDL.h>
#include <vector>
#include <string>
#include "../structures.h"
#include "../ResourceManager.h"

/*********************************************/

#define SIZE_WRAP_CONTENT -1
#define SIZE_MATCH_PARENT -2

/*********************************************/

/*
Calling order:
1. ctor
2. Properties
3. Initialize
4. SetSize
*/

/*********************************************/

class View
{
private:
	//
protected:
	std::string id;
	std::vector<std::string> tags;
	View* parent;
	std::vector<View*> children;
	bool visible;
	Position relativePosition;
	Position absolutePosition;
	Size size;
	Size calculatedSize;
	Size contentSize;
	Box layoutMargin;
	bool isInitialized;
	View* itemTemplate;
public:
	View();
    virtual ~View() {}
	virtual bool Initialize(ResourceManager* resourceManager, SDL_Renderer* renderer) = 0;
	bool InitializeAll(ResourceManager* resourceManager, SDL_Renderer* renderer);
	virtual void Update() = 0;
	virtual void Draw(SDL_Renderer* renderer) = 0;
	std::string GetId();
	void SetId(std::string id);
	std::vector<std::string> GetTags();
	View* GetParentView();
	void SetParentView(View* parent);
	View* GetChildViewById(std::string id);
	View* GetFirstChildViewByTag(std::string tag);
	virtual void AddChildView(View* view);
	int GetNumberOfChildren();
	View* GetChildView(int i);
	bool IsVisible();
	void SetVisible(bool visible);
	Position GetRelativePosition();
	void SetRelativePosition(Position position);
	void SetRelativePosition(int x, int y);
	Position GetAbsolutePosition();
	Size GetSize();
	void SetSize(Size size);
	void SetSize(int width, int height);
	Size GetContentSize();
	Box GetLayoutMargin();
	Size CalculateLayout(Position offsetInCurView, Size parentSize);
	void RecalculateLayout();
	void CalculateAbsolutePosition(Position parentPosition);
	virtual void OnLayoutChange();
	virtual bool SetProperty(std::string name, std::string value);
	virtual View* Copy() = 0;
	View* GetItemTemplate();
	void SetItemTemplate(View* itemTemplate);
	virtual void FillData(std::map<std::string, std::string>& data);
	void PropagateStateChange(std::string stateName, std::string stateValue);
	virtual void OnStateChange(std::string stateName, std::string stateValue);
	/*virtual FormElement* GetElement(int position) = 0;
	virtual int GetSelectedPosition() = 0;
	virtual FormElement* SelectNextElement(Direction direction) = 0;
	virtual void SetTransition(TransitionEffect effect, double value) = 0;
	virtual void ClearTransitions();*/
};

/*********************************************/

#endif
