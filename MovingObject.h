#pragma once

#include <utility>

#include "GameObject.h"
#include "MovingBehavior.h"

class MovingObject: public GameObject {
	double speed;

	MovingBehavior* movingBehavior;
protected:
	void SetMovingBehavior(MovingBehavior* movingBehavior);

	Position GetPostitionToAimTarget(
		GameObject* target,
		Position minStartPositions,
		Position maxStartPositions
	);

	double CalcTimeUntilReachingGivenX(const double& givenX) const;
public:
	MovingObject();
	
	~MovingObject();

	const double GetSpeed() const;

	virtual void SetSpeed(const double& speed);

	double CalcXValueByGivenTime(const double& time, const int& direction) const;
	double CalcApproximatedYAxisReachingByGivenX(const double& givenX) const;

	void Update(const double& timeElapsed) override;
	void Draw(CDC& fDC) override {};
};

