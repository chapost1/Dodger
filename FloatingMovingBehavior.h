#pragma once

#include "MovingBehavior.h"

class FloatingMovingBehavior : public MovingBehavior {
public:
	FloatingMovingBehavior() {
		this->SetDirection(Direction{ 2, 1 });
	}

	Position GetPostitionToAimTarget(
		GameObject* source,
		GameObject* target,
		Position minStartPositions,
		Position maxStartPositions
	) override {
		Position position;

		position.y = (maxStartPositions.y + minStartPositions.y) / 2.0;

		Position targetPos = target->GetPosition();
		CSize targetSize = target->GetSize();

		Position targetCenter = { targetPos.x + targetSize.cx / 2.0, targetPos.y + targetSize.cy / 2.0 };

		double yDelta = position.y + source->GetSize().cy / 2.0 - targetCenter.y;

		double xDelta = yDelta * (this->GetDirection().x / this->GetDirection().y) - source->GetSize().cx / 2.0;

		double xPos = targetCenter.x + xDelta;

		if (xPos < minStartPositions.x) {
			xPos = minStartPositions.x;
		}
		else if (maxStartPositions.x < xPos) {
			xPos = maxStartPositions.x;
		}

		position.x = xPos;

		return position;
	}
};
