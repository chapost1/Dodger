#include "pch.h"
#include "MovingBehaviorFactory.h"
#include "VerticalMovingBehavior.h"
#include "HorizontalMovingBehavior.h"
#include "FloatingMovingBehavior.h"
#include "DivingMovingBehavior.h"

MovingBehavior* GenerateMovingBehavior(const int& code) {
	MovingBehavior* selection = nullptr;

	switch (code) {
	case MOVING_BEHAVIORS_VERTICAL: {
		selection = new VerticalMovingBehavior();
		break;
	}
	case MOVING_BEHAVIORS_HORIZONTAL: {
		selection = new HorizontalMovingBehavior();
		break;
	}
	case MOVING_BEHAVIORS_FLOATING: {
		selection = new FloatingMovingBehavior();
		break;
	}
	case MOVING_BEHAVIORS_DIVING: {
		selection = new DivingMovingBehavior();
		break;
	}
	default: exit(EXIT_FAILURE);
	}

	return selection;
}
