#ifndef _RECTANGLE_SHAPE_H_
#define _RECTANGLE_SHAPE_H_

/*********************************************/

#include "../ViewSystem/View.h"
#include "../structures.h"
#include <vector>
#include <string>
#include "../graphics_utils.h"

/*********************************************/

class RectangleShape : public View
{
private:
	Color strokeColor;
	Color fillColor;
	int strokeWidth;
public:
	RectangleShape();
	~RectangleShape();
	bool OnInitialize();
	void OnDraw(SDL_Renderer* renderer, Position offset);
	View* Copy();

	void SetStrokeColor(Color strokeColor);
	void SetFillColor(Color fillColor);
	void SetStrokeWidth(int strokeWidth);
	bool SetProperty(std::string name, std::string value);
};

/*********************************************/

#endif
