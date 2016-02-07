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

#include "Animator.h"

Animator::Animator(double time, TransitionDirection direction)
{
	this->time = time;
	this->direction = direction;

	Reset();
}

void Animator::Update()
{
	double transitionDelta;
	if (time == 0)
		transitionDelta = 1;
	else
		transitionDelta = (1.0 / (double)FPS) / time;

	int dirMod = direction == TransitionDirectionOn ? 1 : -1;
	transitionPosition += transitionDelta * dirMod;
	if (transitionPosition <= 0 && direction == TransitionDirectionOff || transitionPosition >= 1 && direction == TransitionDirectionOn)
	{
		transitionPosition = clip(transitionPosition, 0.0, 1.0);
		done = true;
	}

	OnUpdate();
}

void Animator::OnUpdate()
{
}

void Animator::Reset()
{
	done = false;
	transitionPosition = direction == TransitionDirectionOn ? 0 : 1;
}

bool Animator::IsDone()
{
	return done;
}

double Animator::GetProgress()
{
	return transitionPosition;
}
