#pragma once

#include "GameObject.h"

struct Direction {
	double x, y;
};

class MovingBehavior {
	Direction direction{ 0, 0 };
public:
	virtual ~MovingBehavior() {}

	void SetDirection(const Direction& direction) {
		double vectorNorm = sqrt(direction.x * direction.x + direction.y * direction.y);
		this->direction.x = direction.x / vectorNorm;
		this->direction.y = direction.y / vectorNorm;
	}

	const Direction& GetDirection() const {
		return this->direction;
	}

	virtual Position Move(const double& directionSpeed, const Position& lastPosition) {
		Position newPosition;
		newPosition.x = lastPosition.x + this->GetDirection().x * directionSpeed;
		newPosition.y = lastPosition.y + this->GetDirection().y * directionSpeed;
		return newPosition;
	}

	virtual double CalcApproximatedYAxisReachingByGivenX(
		const double& directionSpeed,
		const Position& startPosition,
		const double& givenX
	) {
		if (this->GetDirection().x == 0 || this->GetDirection().y == 0) {
			return startPosition.y;
		}
		const double deltaX = abs(givenX - startPosition.x);
		double approximatedYDelta = deltaX * (this->GetDirection().y / this->GetDirection().x);

		if (directionSpeed < 0) {
			approximatedYDelta = -approximatedYDelta;
		}

		return startPosition.y + approximatedYDelta;
	}

	virtual double CalcTimeUntilReachingGivenX(const Position& startPosition, const double& givenX) {
		double movementRatio = abs(this->GetDirection().x);
		if (movementRatio == 0.0) return -1;
		const double deltaX = abs(startPosition.x - givenX);
		if (deltaX == 0.0) return 0;

		return deltaX / movementRatio;
	}

	virtual double CalcXValueByGivenMovesCount(
		const Position& startPosition,
		const double& time,
		const int& direction
	) {
		if (time < 0) return startPosition.x;
		return startPosition.x + this->GetDirection().x * time * direction;
	}

	virtual Position GetPostitionToAimTarget(
		GameObject* source,
		GameObject* target,
		Position minStartPositions,
		Position maxStartPositions
	) = 0;
};
