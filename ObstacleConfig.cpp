#include "pch.h"
#include "ObstacleConfig.h"
#include "GameConfig.h"
#include "MovingBehaviorFactory.h"
#include "LuckUtils.h"

ObstacleConfig::ObstacleConfig(
	const CString paths[],
	const int& framesCount,
	const double& frameSkipThreshold,
	const CSize& imageSize,
	const int& collisionMargin,
	const BOOL& reverseFramesContext,
	const double& maxSpeed,
	const double& initialSpeed,
	const int& movingBehaviourCode,
	const CRect& existenceArea,
	const BOOL& flyAnimation,
	const BOOL& canAimCharacter,
	const BOOL& randomStartPosition,
	const Position& minStartPositions,
	const Position& maxStartPositions
) {
	for (int i = 0; i < framesCount; i++) {
		frameImagesPaths[i] = paths[i];
	}

	this->framesCount = framesCount;
	this->frameSkipThreshold = frameSkipThreshold;

	this->imageSize = imageSize;
	this->collisionMargin = collisionMargin;
	this->reverseFramesContext = reverseFramesContext;

	this->maxSpeed = maxSpeed;

	this->initialSpeed = initialSpeed;

	this->movingBehaviourCode = movingBehaviourCode;

	this->existenceArea = existenceArea;

	this->flyAnimation = flyAnimation;

	this->canAimCharacter = canAimCharacter;
	this->randomStartPosition = randomStartPosition;
	this->minStartPositions = minStartPositions;
	this->maxStartPositions = maxStartPositions;
}

ObstacleConfig enhancedBuildConfig(
	const CString paths[],
	const int& framesCount,
	const double& frameSkipThreshold,
	const CSize& imageSize,
	const int& collisionMargin,
	const BOOL& reverseFramesContext,
	const double& maxSpeed,
	const double& initialSpeed,
	const int& movingBehaviourCode,
	const CRect& existenceArea,
	const BOOL& flyAnimation,
	const BOOL& canAimCharacter,
	const BOOL& randomStartPosition,
	Position minStartPositions,
	Position maxStartPositions,
	const double& aggregatedSpeed
) {
	const double maxSpeedFactor = maxSpeed / initialSpeed;

	double speedFactor = aggregatedSpeed;
	if (maxSpeedFactor < speedFactor) {
		speedFactor = maxSpeedFactor;
	}

	return ObstacleConfig(
		paths,
		framesCount,
		frameSkipThreshold / (speedFactor / 2.0),
		imageSize,
		collisionMargin,
		reverseFramesContext,
		-maxSpeed,
		-initialSpeed * speedFactor,
		movingBehaviourCode,
		existenceArea,
		flyAnimation,
		canAimCharacter,
		randomStartPosition,
		minStartPositions,
		maxStartPositions
	);
}

ObstacleConfig GetGastlyConfig(const double& aggregatedSpeed, const CRect& existenceArea) {
	CString paths[] = {
		_T("./assets/obstacles/gastly/gastly-frame-1.png"),
		_T("./assets/obstacles/gastly/gastly-frame-2.png"),
		_T("./assets/obstacles/gastly/gastly-frame-3.png"),
	};

	return enhancedBuildConfig(
		paths,
		3,
		0.3,
		CSize{ (int)(375 * 0.35), (int)(345 * 0.35) },
		25,
		TRUE,
		OBJECTS_SPEED_FACTOR * 60,
		OBJECTS_SPEED_FACTOR * 20,
		MOVING_BEHAVIORS_HORIZONTAL,
		existenceArea,
		TRUE,
		TRUE,
		TRUE,
		{ existenceArea.Width() + 100.0 , (double)existenceArea.top },
		{ existenceArea.Width() + 100.0, existenceArea.Height() - 345 * 0.35 },
		aggregatedSpeed
	);
}


ObstacleConfig GetDrifloonConfig(const double& aggregatedSpeed, const CRect& existenceArea) {
	CString paths[] = {
		_T("./assets/obstacles/drifloon/drifloon-frame-1.png"),
		_T("./assets/obstacles/drifloon/drifloon-frame-2.png"),
		_T("./assets/obstacles/drifloon/drifloon-frame-3.png"),
	};

	return enhancedBuildConfig(
		paths,
		3,
		0.35,
		CSize{ (int)(238 * 0.34) , (int)(379 * 0.34) },
		23,
		TRUE,
		OBJECTS_SPEED_FACTOR * 30,
		OBJECTS_SPEED_FACTOR * 12,
		MOVING_BEHAVIORS_FLOATING,
		existenceArea,
		TRUE,
		TRUE,
		TRUE,
		{ existenceArea.Width() - 750.0 , existenceArea.Height() + 120.0 },
		{ existenceArea.Width() - 150.0, existenceArea.Height() + 120.0 },
		aggregatedSpeed
	);
}

ObstacleConfig GetTornadusConfig(const double& aggregatedSpeed, const CRect& existenceArea) {
	CString paths[] = {
		_T("./assets/obstacles/tornadus/tornadus-frame-1.png"),
		_T("./assets/obstacles/tornadus/tornadus-frame-2.png"),
		_T("./assets/obstacles/tornadus/tornadus-frame-3.png"),
		_T("./assets/obstacles/tornadus/tornadus-frame-4.png"),
	};

	return enhancedBuildConfig(
		paths,
		4,
		0.3,
		CSize{ (int)(133 * 0.90) , (int)(158 * 0.90) },
		15,
		FALSE,
		OBJECTS_SPEED_FACTOR * 40,
		OBJECTS_SPEED_FACTOR * 16,
		MOVING_BEHAVIORS_HORIZONTAL,
		existenceArea,
		TRUE,
		TRUE,
		TRUE,
		{ existenceArea.Width() + 100.0 , (double)existenceArea.top },
		{ existenceArea.Width() + 100.0, existenceArea.Height() - 158 * 0.90 },
		aggregatedSpeed
	);
}

ObstacleConfig GetZubatConfig(const double& aggregatedSpeed, const CRect& existenceArea) {
	CString paths[] = {
		_T("./assets/obstacles/zubat/zubat-frame-1.png"),
		_T("./assets/obstacles/zubat/zubat-frame-2.png"),
		_T("./assets/obstacles/zubat/zubat-frame-3.png"),
	};

	return enhancedBuildConfig(
		paths,
		3,
		0.075,
		CSize{ (int)(200 * 0.75) , (int)(120 * 0.75) },
		37,
		TRUE,
		OBJECTS_SPEED_FACTOR * 90,
		OBJECTS_SPEED_FACTOR * 45,
		MOVING_BEHAVIORS_DIVING,
		existenceArea,
		TRUE,
		TRUE,
		TRUE,
		{ existenceArea.Width() - 750.0 , existenceArea.top - 100.0 },
		{ existenceArea.Width() - 55.0, existenceArea.top - 100.0 },
		aggregatedSpeed
	);
}

ObstacleConfig GetPokeBallConfig(const double& aggregatedSpeed, const CRect& existenceArea) {
	CString paths[] = {
		_T("./assets/obstacles/pokeball/poke_ball_1.png"),
		_T("./assets/obstacles/pokeball/poke_ball_2.png"),
		_T("./assets/obstacles/pokeball/poke_ball_3.png"),
		_T("./assets/obstacles/pokeball/poke_ball_4.png"),
		_T("./assets/obstacles/pokeball/poke_ball_5.png"),
		_T("./assets/obstacles/pokeball/poke_ball_6.png"),
	};

	int randomFlag = LuckUtils::RandomInRange(0, 2);

	int movingBehaviorFlag = -1;
	Position minStartPositions = { -1, -1 };
	Position maxStartPositions = { -1, -1 };

	double maxSpeed = 0;
	double initialSpeed = 0;

	switch (randomFlag) {
	case 0: {
		movingBehaviorFlag = MOVING_BEHAVIORS_HORIZONTAL;
		minStartPositions = { existenceArea.Width() + 100.0, (double)existenceArea.top };
		maxStartPositions = { existenceArea.Width() + 100.0, existenceArea.Height() - 86 * 0.50 };
		maxSpeed = OBJECTS_SPEED_FACTOR * 140;
		initialSpeed = OBJECTS_SPEED_FACTOR * 70;
		break;
	};
	case 1: {
		movingBehaviorFlag = MOVING_BEHAVIORS_DIVING;
		minStartPositions = { existenceArea.Width() - 650.0, existenceArea.top - 100.0 };
		maxStartPositions = { (double)existenceArea.Width(), existenceArea.top - 100.0 };
		maxSpeed = OBJECTS_SPEED_FACTOR * 120;
		initialSpeed = OBJECTS_SPEED_FACTOR * 60;
		break;
	};
	case 2: {
		movingBehaviorFlag = MOVING_BEHAVIORS_FLOATING;
		minStartPositions = { existenceArea.Width() - 750.0, existenceArea.Height() + 120.0 };
		maxStartPositions = { existenceArea.Width() - 150.0, existenceArea.Height() + 120.0 };
		maxSpeed = OBJECTS_SPEED_FACTOR * 120;
		initialSpeed = OBJECTS_SPEED_FACTOR * 60;
		break;
	};
	}

	return enhancedBuildConfig(
		paths,
		6,
		0.025,
		CSize{ (int)(86 * 0.50) , (int)(86 * 0.50) },
		12,
		FALSE,
		maxSpeed,
		initialSpeed,
		movingBehaviorFlag,
		existenceArea,
		TRUE,
		TRUE,
		TRUE,
		minStartPositions,
		maxStartPositions,
		aggregatedSpeed
	);
}
