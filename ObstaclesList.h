#pragma once

#include "Obstacle.h"
#include <list>

class ObstaclesList {
	std::list<Obstacle*> list;
public:
	virtual ~ObstaclesList();

	void Clear();

	int EraseMarkedForDeletion();

	void Insert(Obstacle* obstacle);

	std::list<Obstacle*> GetList();
	size_t Size() const;

	void Update(const double& timeElapsed);
	void Draw(CDC& fDC);
};
