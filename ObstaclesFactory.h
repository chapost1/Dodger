#pragma once

#include <list>
#include "Obstacle.h"
#include "ObstaclesList.h"

class ObstaclesFactory {
	double minTimeToSpawn;
	double timeToSpawn;
	double currTimeToSpawn;

	int lastRandomFlag;

	BOOL isReadyToSpawn;

	void linkNewObstacleToList(
		Character* character,
		ObstaclesList& obstaclesList,
		const CRect& playground,
		const double& aggregatedSpeed
	);

	Obstacle* getNewRandomObstacle(
		Character* character,
		const CRect& playground,
		const double& aggregatedSpeed
	);

	Position getBestRandomPositionForNewObstacle(
		Character* character,
		const CRect& playground,
		std::list<Obstacle*> obstaclesToCompare,
		Obstacle* obstacle
	);
public:
	ObstaclesFactory();

	void NewObstacleSpawn(
		Character* character,
		ObstaclesList& obstaclesList,
		const CRect& playground,
		const double& aggregatedSpeed,
		const BOOL& sudo = FALSE 
	);

	size_t CountProblematicObstaclesComboForUserByGivenObstacleSpawn(
		std::list<Obstacle*> obstaclesToCompare,
		Character* character,
		Obstacle* obstacle,
		double& smallestYDelta
	);

	void Reset();

	void Update(const double& timeElapsed);
};
