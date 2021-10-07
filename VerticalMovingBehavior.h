#pragma once

#include "MovingBehavior.h"

class VerticalMovingBehavior : public MovingBehavior {
public:
	VerticalMovingBehavior() {
		this->SetDirection(Direction{ 0, 1 });
	}

	Position GetPostitionToAimTarget(
		GameObject* source,
		GameObject* target,
		Position minStartPositions,
		Position maxStartPositions
	) override {
		return target->GetPosition();
	}
};