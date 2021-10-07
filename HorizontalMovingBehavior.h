#pragma once

#include "MovingBehavior.h"

class HorizontalMovingBehavior : public MovingBehavior {
public:
	HorizontalMovingBehavior() {
		this->SetDirection(Direction{ 1, 0 });
	}

	Position GetPostitionToAimTarget(
		GameObject* source,
		GameObject* target,
		Position minStartPositions,
		Position maxStartPositions
	) override {
		Position position;

		position.x = (maxStartPositions.x + minStartPositions.x) / 2.0;

		Position targetPos = target->GetPosition();
		CSize targetSize = target->GetSize();

		double targetYCenter = targetPos.y + targetSize.cy / 2.0;

		position.y = targetYCenter - source->GetSize().cy / 2.0;

		if (position.y < minStartPositions.y) {
			position.y = minStartPositions.y;
		}
		else if (maxStartPositions.y < position.y) {
			position.y = maxStartPositions.y;
		}

		return position;
	}
};