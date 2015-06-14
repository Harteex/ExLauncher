#ifndef _ANIMATOR_H_
#define _ANIMATOR_H_

/*********************************************/

#include <SDL.h>
#include <vector>
#include <string>
#include "utils.h"
#include "structures.h"
#include "global.h"

/*********************************************/

enum TransitionDirection { TransitionDirectionOn, TransitionDirectionOff };

/*********************************************/

class Animator
{
protected:
	double time;
	TransitionDirection direction;
	bool done;
	double transitionPosition;

	virtual void OnUpdate();
public:
	Animator(double time, TransitionDirection direction);
	virtual ~Animator() {}
	void Update();
	void Reset();
	bool IsDone();
	double GetProgress();
};

/*********************************************/

#endif
