#include "pch.h"
#include "MovingObject.h"

MovingObject::MovingObject()
	:GameObject() {
	this->movingBehavior = NULL;
	this->SetSpeed(0);
}

MovingObject::~MovingObject() {
	delete this->movingBehavior;
}

const double MovingObject::GetSpeed() const {
	return this->speed;
}

void MovingObject::SetMovingBehavior(MovingBehavior* movingBehavior) {
	delete this->movingBehavior;
	this->movingBehavior = movingBehavior;
}

void MovingObject::SetSpeed(const double& speed) {
	this->speed = speed;
}

void MovingObject::Update(const double& timeElapsed) {
	Position nextPosition = this->movingBehavior->Move(timeElapsed * this->GetSpeed(), this->GetPosition());
	this->SetPosition(nextPosition);
}

double MovingObject::CalcApproximatedYAxisReachingByGivenX(const double& givenX) const {
	return this->movingBehavior->CalcApproximatedYAxisReachingByGivenX(
			this->GetSpeed(),
			this->GetPosition(),
			givenX
		);
}

double MovingObject::CalcTimeUntilReachingGivenX(const double& givenX) const {
	double pureMovements =
		this->movingBehavior->CalcTimeUntilReachingGivenX(this->GetPosition(), givenX);

	double speedFactor = abs(this->GetSpeed());
	if (speedFactor == 0) return pureMovements;
	return (pureMovements / speedFactor);
}

double MovingObject::CalcXValueByGivenTime(const double& time, const int& direction) const {
	return this->movingBehavior->CalcXValueByGivenMovesCount(
		this->GetPosition(),
		abs(this->GetSpeed()) * time,
		direction
	);
}

Position MovingObject::GetPostitionToAimTarget(
	GameObject* target,
	Position minStartPositions,
	Position maxStartPositions
) {
	return this->movingBehavior->GetPostitionToAimTarget(
		this,
		target,
		minStartPositions,
		maxStartPositions
	);
}
