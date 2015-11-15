#ifndef _VIEW_H_
#define _VIEW_H_

/*********************************************/

#include <SDL.h>
#include <vector>
#include <string>
#include "../structures.h"
#include "../ResourceManager.h"
#include "../ScreenSystem/InputState.h"

/*********************************************/

class Screen;

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
	void DrawBackground(Position offset);
	void DrawDebugViewBounds(Rectangle viewBounds);
	Rectangle CalculateViewBounds(Position offset, Rectangle parentViewBounds);
	bool isInitialized;
protected:
	Screen* context;
	std::string id;
	std::string name;
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
	int layoutGravity;
	int gravity;
	View* itemTemplate;
	Color background;
	std::string action;

	View* CopyBase(View* view);
	virtual void DrawChildren(Position offset, Rectangle viewBounds);
	std::string FindAndReplace(std::string origString, std::map<std::string, std::string>& values);
public:
	View();
    virtual ~View() {}
	bool Initialize(Screen* context);
	virtual bool OnInitialize();
	bool InitializeAll(Screen* context);
	bool IsInitialized();
	void Update();
	virtual void OnUpdate();
	virtual void Draw(Position offset = Position(0, 0), Rectangle parentViewBounds = Rectangle(-1, -1, -1, -1));
	virtual void OnDraw(SDL_Renderer* renderer, Position offset);
	std::string GetId();
	void SetId(std::string id);
	std::string GetName();
	void SetName(std::string name);
	std::vector<std::string> GetTags();
	View* GetParentView();
	void SetParentView(View* parent);
	View* GetChildViewById(std::string id);
	View* GetFirstChildViewByTag(std::string tag);
	virtual void AddChildView(View* view);
	int GetNumberOfChildren();
	View* GetChildView(int i);
	void SortChildren(bool(*comparer)(View* v1, View* v2));
	bool IsVisible();
	void SetVisible(bool visible);
	Position GetRelativePosition();
	void SetRelativePosition(Position position);
	void SetRelativePosition(int x, int y);
	Position GetAbsolutePosition();
	Size GetSize();
	void SetSize(Size size);
	void SetSize(int width, int height);
	Size GetCalculatedSize();
	Size GetContentSize();
	Rectangle GetViewBounds();
	Box GetLayoutMargin();
	void SetLayoutMargin(Box margin);
	int GetLayoutGravity();
	void SetLayoutGravity(int gravity);
	int GetGravity();
	void SetGravity(int gravity);
	std::string GetAction();
	void SetAction(std::string action);
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
	Position GetGravityOffset(Size childSize, Size containerSize, int childLayoutGravity);
	/*virtual void SetTransition(TransitionEffect effect, double value) = 0;
	virtual void ClearTransitions();*/
};

/*********************************************/

#endif
