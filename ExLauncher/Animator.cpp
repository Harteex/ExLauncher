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
