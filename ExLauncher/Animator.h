/*
Copyright 2016 Andreas Bjerkeholt

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

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
