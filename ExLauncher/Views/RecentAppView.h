#ifndef _RECENT_APP_VIEW_H_
#define _RECENT_APP_VIEW_H_

/*********************************************/

#include "../ViewSystem/View.h"
#include "../structures.h"
#include "FramePanel.h"

/*********************************************/

class RecentAppView : public FramePanel
{
private:
	int recentIndex;
	std::string category;
	View* origView;

	void FillViewWithRecent();
	void OnRecentChanged();
public:
	RecentAppView();
	~RecentAppView();
	bool OnInitialize();
	View* Copy();
	void AddChildView(View* view);

	void SetRecentIndex(int recentIndex);
	void SetCategory(std::string category);
	bool SetProperty(std::string name, std::string value);
};

/*********************************************/

#endif
