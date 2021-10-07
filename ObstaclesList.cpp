#include "pch.h"
#include "ObstaclesList.h"

ObstaclesList::~ObstaclesList() {
	this->Clear();
}

std::list<Obstacle*> ObstaclesList::GetList() {
	return this->list;
}

size_t ObstaclesList::Size() const {
	return this->list.size();
}

void ObstaclesList::Clear() {
	std::list<Obstacle*>::iterator it = this->list.begin();
	while (it != this->list.end()) {
		delete* it;
		it++;
	}
	this->list.clear();
}

int ObstaclesList::EraseMarkedForDeletion() {
	std::list<Obstacle*> markedItemsList;
	
	std::list<Obstacle*>::iterator it = this->list.begin();
	while (it != this->list.end()) {
		if ((*it)->GetMarkedForDeletion()) {
			markedItemsList.push_back(*it);
		}
		it++;
	}

	it = markedItemsList.begin();
	while (it != markedItemsList.end()) {
		delete* it;
		this->list.remove(*it);
		it++;
	}

	return markedItemsList.size();
}

void ObstaclesList::Insert(Obstacle* obstacle) {
	this->list.push_back(obstacle);
}

void ObstaclesList::Update(const double& timeElapsed) {
	std::list<Obstacle*>::iterator it = this->list.begin();
	while (it != this->list.end()) {
		(*it)->Update(timeElapsed);
		it++;
	}
}

void ObstaclesList::Draw(CDC& fDC) {
	std::list<Obstacle*> running;
	std::list<Obstacle*> flying;

	std::list<Obstacle*>::iterator it = this->list.begin();
	while (it != this->list.end()) {
		if ((*it)->IsFlying()) {
			flying.push_back(*it);
		}
		else {
			running.push_back(*it);
		}
		it++;
	}

	it = running.begin();
	while (it != running.end()) {
		(*it)->Draw(fDC);
		it++;
	}
	it = flying.begin();
	while (it != flying.end()) {
		(*it)->Draw(fDC);
		it++;
	}
}
