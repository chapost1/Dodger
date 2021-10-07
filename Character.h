#pragma once

#include "MovingObject.h"
#include "FramesIterator.h"

class Character: public MovingObject
{
	static double SPEED;
	FramesIterator frames;
	BOOL killed;

	BOOL detectWallsCollisions() const;

public:
	Character();

	BOOL IsAlive() const;

	void Reset();

	void Kill();

	void MoveUp();
	void MoveDown();
	void Stop();

	void Update(const double& timeElapsed) override;
	void Draw(CDC& dc) override;
};

