#pragma once

#include "GameObject.h"

class ObstacleConfig {
public:
	CString frameImagesPaths[10];
	int framesCount;
	double frameSkipThreshold;

	CSize imageSize;
	int collisionMargin;
	BOOL reverseFramesContext;

	double maxSpeed;

	double initialSpeed;

	int movingBehaviourCode;

	CRect existenceArea;

	BOOL flyAnimation;

	BOOL canAimCharacter;
	BOOL randomStartPosition;
	Position minStartPositions;
	Position maxStartPositions;

	ObstacleConfig(
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
	);
};

ObstacleConfig GetGastlyConfig(const double& aggregatedSpeed, const CRect& existenceArea);
ObstacleConfig GetDrifloonConfig(const double& aggregatedSpeed, const CRect& existenceArea);
ObstacleConfig GetTornadusConfig(const double& aggregatedSpeed, const CRect& existenceArea);
ObstacleConfig GetZubatConfig(const double& aggregatedSpeed, const CRect& existenceArea);
ObstacleConfig GetPokeBallConfig(const double& aggregatedSpeed, const CRect& existenceArea);
