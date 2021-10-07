#include "pch.h"
#include "ObstaclesFactory.h"

ObstaclesFactory::ObstaclesFactory(): lastRandomFlag(-1) {
	this->minTimeToSpawn = 1.5;
	this->isReadyToSpawn = FALSE;
	this->Reset();
}

void ObstaclesFactory::Reset() {
	this->timeToSpawn = 3.0;
	this->currTimeToSpawn = 0;
}

void ObstaclesFactory::Update(const double& timeElapsed) {
	if (this->timeToSpawn >= this->minTimeToSpawn) {
		this->timeToSpawn -= timeElapsed / 100.0;
	}

	this->currTimeToSpawn -= timeElapsed;

	if (this->currTimeToSpawn <= 0) {
		this->isReadyToSpawn = TRUE;
		this->currTimeToSpawn = timeToSpawn;
	}
}

void ObstaclesFactory::NewObstacleSpawn(
	Character* character,
	ObstaclesList& obstaclesList,
	const CRect& playground,
	const double& aggregatedSpeed,
	const BOOL& sudo
) {
	if (!sudo) {
		if (!this->isReadyToSpawn) return;

		this->isReadyToSpawn = FALSE;
	}

	this->linkNewObstacleToList(character, obstaclesList, playground, aggregatedSpeed);
}

void ObstaclesFactory::linkNewObstacleToList(
	Character* character,
	ObstaclesList& obstaclesList,
	const CRect& playground,
	const double& aggregatedSpeed
) {
	Obstacle* obstacle = this->getNewRandomObstacle(character, playground, aggregatedSpeed);
	if (!obstacle) return;

	if (// no random supported for this obstacle type anyway
		!obstacle->isRandomPositionSupported() ||
		obstaclesList.Size() == 0// no more obstacles so it doesn't even matter
	) {
		obstaclesList.Insert(obstacle);
		return;
	}

	obstacle->SetPosition(
		getBestRandomPositionForNewObstacle(character, playground, obstaclesList.GetList(), obstacle)
	);

	obstaclesList.Insert(obstacle);
}

Obstacle* ObstaclesFactory::getNewRandomObstacle(
	Character* character,
	const CRect& playground,
	const double& aggregatedSpeed
) {
	int randomFlag = -1;
	// keeps the obstacle random from spawn to spawn
	do {
		randomFlag = LuckUtils::RandomInRange(0, 4);
	} while (randomFlag == this->lastRandomFlag);

	this->lastRandomFlag = randomFlag;

	// create the obstacle using the random flag
	Obstacle* obstacle = nullptr;

	switch (randomFlag) {
	case 0: {
		obstacle = new Obstacle(
			character,
			GetGastlyConfig(aggregatedSpeed, playground)
		);
		break;
	};
	case 1: {
		obstacle = new Obstacle(
			character,
			GetTornadusConfig(aggregatedSpeed, playground)
		);
		break;
	};
	case 2: {
		obstacle = new Obstacle(
			character,
			GetDrifloonConfig(aggregatedSpeed, playground)
		);
		break;
	};
	case 3: {
		obstacle = new Obstacle(
			character,
			GetZubatConfig(aggregatedSpeed, playground)
		);
		break;
	};
	case 4: {
		obstacle = new Obstacle(
			character,
			GetPokeBallConfig(aggregatedSpeed, playground)
		);
		break;
	};
	default: {
		return nullptr;
	}
	}
	return obstacle;
}

Position ObstaclesFactory::getBestRandomPositionForNewObstacle(
	Character* character,
	const CRect& playground,
	std::list<Obstacle*> obstaclesToCompare,
	Obstacle* obstacle
) {
	const int playgroundArea = playground.Height() * playground.Width();
	const int obstacleArea = obstacle->GetSize().cx * obstacle->GetSize().cy;
	// retries is the amount of time that the playground can contain the obstacle * 4.0 for corners
	// looking at the Probability prespective, it should be enough
	int retriesLeft = (int)(playgroundArea / obstacleArea) * 4;

	// record initial random option
	Position bestPosition = obstacle->GetPosition();
	double bestPositionSmallestYDelta = -1;
	size_t bestPositionProblemsCount =
		this->CountProblematicObstaclesComboForUserByGivenObstacleSpawn(
			obstaclesToCompare,
			character,
			obstacle,
			bestPositionSmallestYDelta
		);
	// try until you've found a perfect location OR no more retries
	while (0 < bestPositionProblemsCount && 0 < retriesLeft) {
		// investigate another new random pos
		obstacle->SetRandomPosition();

		double smallestYDelta = -1;
		size_t problemsCount =
			this->CountProblematicObstaclesComboForUserByGivenObstacleSpawn(
				obstaclesToCompare,
				character,
				obstacle,
				smallestYDelta
			);

		BOOL recordNewPosition = FALSE;

		// always record the least problematic option so we can use it afterwards
		if (problemsCount < bestPositionProblemsCount) {
			recordNewPosition = TRUE;
		}
		// if it has the same problems count, another important factor is:
		// the new position is leaving the user more Y for moving between the problems
		// so if the new has more delta, the user have more space to move
		else if (
			problemsCount == bestPositionProblemsCount &&
			bestPositionSmallestYDelta < smallestYDelta
			) {
			recordNewPosition = TRUE;
		}
		// record new position if it's better than prev
		if (recordNewPosition) {
			bestPosition = obstacle->GetPosition();
			bestPositionProblemsCount = problemsCount;
			bestPositionSmallestYDelta = smallestYDelta;
		}
		retriesLeft--;
	}
	return bestPosition;
}

size_t ObstaclesFactory::CountProblematicObstaclesComboForUserByGivenObstacleSpawn(
	std::list<Obstacle*> obstaclesToCompare,
	Character* character,
	Obstacle* obstacle,
	double& smallestYDelta
) {
	const CSize charSize = character->GetSize();
	const double charRightX = character->GetPosition().x + charSize.cx;

	int count = 0;

	smallestYDelta = -1;

	std::list<Obstacle*>::iterator it = obstaclesToCompare.begin();
	while (it != obstaclesToCompare.end()) {
		Obstacle* otherObstacle = *it;

		// if the obstacle already passed the character so he's ir-relevant
		if (otherObstacle->GetPosition().x < charRightX) {
			it++;
			continue;
		}

		/*
			we still want to know if the X pos of both are relevant as a combo
			related to the time one is reaching the user, to know if the
			second one is far away enough so the user can pass
			we compare the X delta between those 
			if the delta is big enough, this is not actually an obstacle which is going to
			appear in front of the user at similar time to candidate one (relative)
		*/
		
		double XDeltaBetweenObstacles;

		// 2 situations:
		// // 1. new obstacle is going to be at the user loc first
		// // 2.other obstacle is the first

		// time until the other obstacle reaches the user
		const double timeUntilOtherObstacleReachingChar =
			otherObstacle->CalcTimeUntilReachingCharacterX();
		// time until the candidate obstacle reaches the user
		const double timeUntilCandidateObstacleReachingChar =
			obstacle->CalcTimeUntilReachingCharacterX();

		// situations blocks:

		// 1. new obstacle will get as second place
		if (timeUntilOtherObstacleReachingChar <= timeUntilCandidateObstacleReachingChar) {
			// candidate obstacle x position at the exact same time
			const double obstacleXAtTheSameTime =
				obstacle->CalcXValueByGivenTime(timeUntilOtherObstacleReachingChar, -1);
			// probably other obstacle is the first
			XDeltaBetweenObstacles =
				abs(charRightX + otherObstacle->GetSize().cx - obstacleXAtTheSameTime);
		}
		// 2. altough new obstacle came second he is faster and reaches the user first
		else {
			// candidate obstacle x position at the exact same time
			const double otherObstacleXAtTheSameTime =
				otherObstacle->CalcXValueByGivenTime(timeUntilCandidateObstacleReachingChar, -1);
			// probably candidate obstacle is the first
			XDeltaBetweenObstacles =
				abs(charRightX + obstacle->GetSize().cx - otherObstacleXAtTheSameTime);
		}
		// after calculating the right X delta..
		// char should be able to pass between them easily, so that's fine
		if (XDeltaBetweenObstacles <= charSize.cx * 2.0) {
			it++;
			continue;
		}

		/*
			we have reached to an obstacle which is
			going to reach the user in similar time to candidate obstacle
			so we would want to check the Y delta between those
			(when reaching the user) so the user won't have too hard time passing between them
		*/

		const double otherObstacleY = otherObstacle->GetApproximatedYAxisWhenReachingCharArea();

		const double obstacleY = obstacle->GetApproximatedYAxisWhenReachingCharArea();

		double yDelta = 0;

		// candidate obstacle is above the loop obstacle
		if (otherObstacleY <= obstacleY + obstacle->GetSize().cy) {
			yDelta = obstacleY + obstacle->GetSize().cy - otherObstacleY;
		}// below
		else if (otherObstacleY + otherObstacle->GetSize().cy <= obstacleY) {
			yDelta = otherObstacleY + otherObstacle->GetSize().cy - obstacleY;
		}

		// delta == 0 means they will be on each other, not optimal but tha't alright
		// char < delta means he will pass it easily
		if (yDelta == 0 || charSize.cy * 1.25 < yDelta) {
			it++;
			continue;
		}

		// second factor: the smallest space for the user to move by the given problematic collisions
		// we record it so if we meet two problems count equally options we chose by space it leave the user
		if (smallestYDelta == -1 || yDelta < smallestYDelta) {
			smallestYDelta = yDelta;
		}

		// user will have an hard time passing though this two obstacles so count it...
		count++;

		it++;
	}

	return count;
}
