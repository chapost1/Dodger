#pragma once

#include <list>

#include "Character.h"
#include "Obstacle.h"
#include "ObstaclesList.h"
#include "ObstaclesFactory.h"
#include "GameNotch.h"
#include "Player.h"
#include "GameConfig.h"
#include "Subject.h"
#include "LuckUtils.h"

class Game: public ISubject<int> {
	int STATE;

	std::list<IObserver<int>*> gameStateSubscribers;

	Player* player;

	Character character;

	ObstaclesFactory obstaclesFactory;

	ObstaclesList obstacles;

	GameNotch notch;

	CRect playground;

	CImage bg;
	CImage instructions;
	FramesIterator gameReadyAnimation;
	FramesIterator gameOverAnimation;

	double bgScrollTreshold;
	int bgScrollPos;

	double timer;

	void loadGameModesBackgrounds();

	void setGameState(const int& STATE);

	void enterGameOverMode();

	void publishGameState();

	void reset();
public:
	Game();

	void Start();

	void AddSubscriber(IObserver<int>* observer) override;
	void RemoveSubscriber(IObserver<int>* observer) override;

	Player* GetPlayer() const;
	void SetPlayer(Player* player);

	void SetScoreToBeat(const unsigned long long& score);

	void SetPlaygroundSize(const CRect& avaiableWindowSize);

	void IncreaseTimer(const double& time);
	void ResetTimer();
	double GetTimer() const;

	void OnKeyDown(UINT key);
	void OnKeyUp(UINT key);

	void FactoryReset();

	void Update(const double& timeElapsed);
	void Draw(CDC& fDC);
};

