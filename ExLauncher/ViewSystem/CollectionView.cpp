#include "CollectionView.h"
#include <algorithm>

using namespace std;

CollectionView::CollectionView()
{
	contentSize.w = 0;
	contentSize.h = 0;

	selectedIndex = 0;
	selectedItem = NULL;
}

void CollectionView::HandleInput(InputState* input)
{
	if (input->GameKeyRepeat(GAMEKEY_UP))
		SelectNext(DirectionUp);
	else if (input->GameKeyRepeat(GAMEKEY_DOWN))
		SelectNext(DirectionDown);
	else if (input->GameKeyRepeat(GAMEKEY_LEFT))
		SelectNext(DirectionLeft);
	else if (input->GameKeyRepeat(GAMEKEY_RIGHT))
		SelectNext(DirectionRight);
}

int CollectionView::GetSelectedIndex()
{
	return selectedIndex;
}

View* CollectionView::GetSelectedView()
{
	if (selectedIndex < 0 || selectedIndex >= children.size())
		return NULL;

	return children[selectedIndex];
}

bool CollectionView::SelectByName(string name)
{
	for (int i = 0; i < children.size(); i++)
	{
		View* v = children[i];

		if (v->GetName() == name)
			return SelectByIndex(i);
	}

	// No view with the specified name was found
	return false;
}

bool CollectionView::SelectByIndex(int index)
{
	if (index < 0 || index >= children.size())
		return false;

	PropagateStateChange("stateSelected", "false");
	children[index]->PropagateStateChange("stateSelected", "true");

	selectedIndex = index;
	OnSelectionChanged();
	return true;
}
