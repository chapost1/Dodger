#pragma once

#include "MovingObject.h"
#include "FramesIterator.h"
#include "Character.h"
#include "LuckUtils.h"
#include "ObstacleConfig.h"

class Obstacle: public MovingObject {
	BOOL markedForDeletion;
	double maxSpeed;
	int collisionMargin;
	FramesIterator frames;
	Character* character;

	BOOL flyAnimation;

	BOOL randomStartPosition;
	Position minStartPositions;
	Position maxStartPositions;

	BOOL collisionsHandler();

	BOOL detectCollisionsWithCharacter() const;
	BOOL isOutOfFrame() const;

	BOOL isInsideCharacterArea(const Position& point) const;

	void aimCharacter();
public:
	Obstacle(Character* character, const ObstacleConfig& config);

	BOOL IsFlying() const;

	BOOL GetMarkedForDeletion() const;
	void SetMarkedForDeletion(const BOOL& flag);

	double GetMaxSpeed() const;
	void SetMaxSpeed(const double& speed);

	void SetSpeed(const double& speed) override;

	BOOL isRandomPositionSupported() const;

	void SetRandomPosition();

	Character* GetCharacter() const;
	void SetCharacter(Character* character);

	double GetApproximatedYAxisWhenReachingCharArea() const;

	double CalcTimeUntilReachingCharacterX() const;

	void Update(const double& timeElapsed) override;
	void Draw(CDC& dc) override;
};
