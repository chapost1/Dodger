#include "pch.h"
#include "Obstacle.h"
#include "MovingBehaviorFactory.h"

Obstacle::Obstacle(Character* character, const ObstacleConfig& config) : 
	frames(config.reverseFramesContext),
	MovingObject()
{
	this->SetMarkedForDeletion(FALSE);

	this->SetMovingBehavior(GenerateMovingBehavior(config.movingBehaviourCode));

	this->SetSize(config.imageSize);

	this->frames.Load(config.frameImagesPaths, config.framesCount, config.frameSkipThreshold);

	this->SetMaxSpeed(config.maxSpeed);

	this->SetSpeed(config.initialSpeed);

	this->SetCharacter(character);

	this->SetExistenceArea(config.existenceArea);

	this->flyAnimation = config.flyAnimation;

	this->collisionMargin = config.collisionMargin;

	this->randomStartPosition = config.randomStartPosition;
	this->minStartPositions = config.minStartPositions;
	this->maxStartPositions = config.maxStartPositions;

	// start pos
	// 66%
	if (config.canAimCharacter && LuckUtils::FailRouletteSpin(3)) {
		this->aimCharacter();
	}
	else {
		this->SetRandomPosition();
	}
}

BOOL Obstacle::isRandomPositionSupported() const {
	return this->randomStartPosition;
}

void Obstacle::aimCharacter() {
	this->SetPosition(
		this->GetPostitionToAimTarget(
			this->GetCharacter(),
			this->minStartPositions,
			this->maxStartPositions
		)
	);
}

void Obstacle::SetRandomPosition() {
	Position position;

	position.x = (double)LuckUtils::RandomInRange(
		(int)this->minStartPositions.x,
		(int)this->maxStartPositions.x
	);

	position.y = (double)LuckUtils::RandomInRange(
		(int)this->minStartPositions.y,
		(int)this->maxStartPositions.y
	);

	this->SetPosition(position);
}

BOOL Obstacle::IsFlying() const {
	return this->flyAnimation;
}

double Obstacle::GetApproximatedYAxisWhenReachingCharArea() const {
	const double charRightX = this->GetCharacter()->GetPosition().x + this->GetCharacter()->GetSize().cx;
	return this->CalcApproximatedYAxisReachingByGivenX(charRightX);
}

BOOL Obstacle::GetMarkedForDeletion() const {
	return this->markedForDeletion;
}

void Obstacle::SetMarkedForDeletion(const BOOL& flag) {
	this->markedForDeletion = flag;
}

double Obstacle::GetMaxSpeed() const {
	return this->maxSpeed;
}

void Obstacle::SetMaxSpeed(const double& speed) {
	this->maxSpeed = speed;
}

void Obstacle::SetSpeed(const double& speed) {
	if (this->GetMaxSpeed()<= speed) {
		MovingObject::SetSpeed(speed);
	}
	else {
		MovingObject::SetSpeed(this->GetMaxSpeed());
	}
}

Character* Obstacle::GetCharacter() const {
	return this->character;
}

void Obstacle::SetCharacter(Character* character) {
	this->character = character;
}

void Obstacle::Update(const double& timeElapsed) {
	if (!this->collisionsHandler()) {
		MovingObject::Update(timeElapsed);
	}

	this->frames.Update(timeElapsed);
}

void Obstacle::Draw(CDC& dc) {
	this->frames.GetFrame()->Draw(
		dc.GetSafeHdc(),
		(int)this->GetPosition().x,
		(int)this->GetPosition().y,
		this->GetSize().cx,
		this->GetSize().cy
	);
}

BOOL Obstacle::collisionsHandler() {
	if (this->detectCollisionsWithCharacter()) {
		this->GetCharacter()->Kill();
		return TRUE;
	} 
	if (this->isOutOfFrame()) {
		this->SetMarkedForDeletion(TRUE);
		return TRUE;
	}
	return FALSE;
}

BOOL Obstacle::isOutOfFrame() const {
	return this->GetPosition().x + this->GetSize().cx < this->GetExistenceArea().left;
}

BOOL Obstacle::detectCollisionsWithCharacter() const {
	Position topLeftPoint = this->GetPosition();
	Position bottomLeftPoint = Position{ topLeftPoint.x, topLeftPoint.y + this->GetSize().cy };
	Position centerLeftPoint = Position{ topLeftPoint.x, topLeftPoint.y + this->GetSize().cy / 2 };
	Position topCenterPoint = Position{ topLeftPoint.x + this->GetSize().cx / 2, topLeftPoint.y };
	Position topRightPoint = Position{ topLeftPoint.x + this->GetSize().cx, topLeftPoint.y };
	Position bottomRightPoint = Position{ topLeftPoint.x + this->GetSize().cx, topLeftPoint.y + this->GetSize().cy };
	Position centerRightPoint = Position{ topLeftPoint.x + this->GetSize().cx, topLeftPoint.y + this->GetSize().cy / 2 };
	Position bottomCenterPoint = Position{ topLeftPoint.x + this->GetSize().cx / 2, topLeftPoint.y + this->GetSize().cy };

	return (
		this->isInsideCharacterArea(topLeftPoint) ||
		this->isInsideCharacterArea(bottomLeftPoint) ||
		this->isInsideCharacterArea(centerLeftPoint) ||
		this->isInsideCharacterArea(topCenterPoint) ||
		this->isInsideCharacterArea(topRightPoint) ||
		this->isInsideCharacterArea(bottomRightPoint) ||
		this->isInsideCharacterArea(centerRightPoint) ||
		this->isInsideCharacterArea(bottomCenterPoint)
	);
}

BOOL Obstacle::isInsideCharacterArea(const Position& point) const {
	double characterLeft = this->GetCharacter()->GetPosition().x;
	double characterRight = characterLeft + this->GetCharacter()->GetSize().cx;
	double characterTop = this->GetCharacter()->GetPosition().y;
	double characterBottom = characterTop + this->GetCharacter()->GetSize().cy;

	return (
		characterLeft + this->collisionMargin <= point.x &&
		characterRight - this->collisionMargin >= point.x &&
		characterTop + this->collisionMargin <= point.y &&
		characterBottom - this->collisionMargin >= point.y
	);
}

double Obstacle::CalcTimeUntilReachingCharacterX() const {
	const double charRightX = this->GetCharacter()->GetPosition().x + this->GetCharacter()->GetSize().cx;
	return this->CalcTimeUntilReachingGivenX(charRightX);
}
