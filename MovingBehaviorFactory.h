#pragma once

#include "MovingBehavior.h"

enum {
	MOVING_BEHAVIORS_VERTICAL = 1,
	MOVING_BEHAVIORS_HORIZONTAL = 2,
	MOVING_BEHAVIORS_FLOATING = 3,
	MOVING_BEHAVIORS_DIVING = 4,
};

MovingBehavior* GenerateMovingBehavior(const int& code);
