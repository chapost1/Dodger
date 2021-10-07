#include "pch.h"
#include "Game.h"

Game::Game() :
	notch(),
	character(),
	gameOverAnimation(FALSE),
	gameReadyAnimation(FALSE),
	obstaclesFactory()
{
	this->setGameState(INITIAL_GAME_STATE);
	this->loadGameModesBackgrounds();

	this->reset();
}

void Game::loadGameModesBackgrounds() {
	bg.Load(_T("./assets/backgrounds/game_running.jpg"));
	instructions.Load(_T("./assets/backgrounds/instructions.jpg"));

	CString gameReadyFrames[] = {
		_T("./assets/backgrounds/game_ready/game_ready_0.jpg"),
		_T("./assets/backgrounds/game_ready/game_ready_1.jpg"),
		_T("./assets/backgrounds/game_ready/game_ready_2.jpg"),
		_T("./assets/backgrounds/game_ready/game_ready_3.jpg"),
		_T("./assets/backgrounds/game_ready/game_ready_4.jpg"),
		_T("./assets/backgrounds/game_ready/game_ready_5.jpg"),
		_T("./assets/backgrounds/game_ready/game_ready_6.jpg"),
		_T("./assets/backgrounds/game_ready/game_ready_7.jpg"),
	};
	gameReadyAnimation.Load(gameReadyFrames, 8, 0.1);

	CString gameOverFrames[] = {
		_T("./assets/backgrounds/game_over/game_over_0.jpg"),
		_T("./assets/backgrounds/game_over/game_over_1.jpg"),
		_T("./assets/backgrounds/game_over/game_over_2.jpg"),
		_T("./assets/backgrounds/game_over/game_over_3.jpg"),
	};
	gameOverAnimation.Load(gameOverFrames, 4, 0.125 / (3.0 / 2.0));
}

void Game::IncreaseTimer(const double& time) {
	this->timer = this->GetTimer() + time / 100.0;
}

void Game::ResetTimer() {
	this->timer = 1.0;
}

double Game::GetTimer() const {
	return this->timer;
}

void Game::reset() {
	this->ResetTimer();
	this->obstaclesFactory.Reset();
	this->bgScrollTreshold = 0.0;
	this->bgScrollPos = 0;
}

void Game::FactoryReset() {
	this->reset();
	this->SetPlayer(nullptr);
	this->setGameState(INITIAL_GAME_STATE);
}

void Game::Start() {
	if (this->player == nullptr) return;

	this->reset();

	this->notch.Start(this->GetPlayer(), 0);

	this->character.SetExistenceArea(this->playground);

	this->character.Reset();

	this->setGameState(GAME_STATES{ RUNNING });
}

Player* Game::GetPlayer() const {
	return this->player;
}

void Game::SetPlayer(Player* player) {
	this->player = player;
}

void Game::SetScoreToBeat(const unsigned long long& score) {
	this->notch.SetScoreToBeat(score);
}

void Game::SetPlaygroundSize(const CRect& avaiableWindowSize) {
	this->notch.SetWidth(avaiableWindowSize.right - avaiableWindowSize.left);
	this->playground = CRect(0, this->notch.GetHeight(), avaiableWindowSize.right - avaiableWindowSize.left, avaiableWindowSize.bottom - avaiableWindowSize.top);

}

void Game::Update(const double& timeElapsed) {
	if (this->GetPlayer() == nullptr) {
		this->setGameState(GAME_STATES{ INSTRUCTIONS });
		return;
	}
	else if (this->GetPlayer() != nullptr && this->STATE == GAME_STATES{ INSTRUCTIONS }) {
		this->setGameState(GAME_STATES{ GAME_READY });
		return;
	}
	else if (this->STATE == GAME_STATES{ GAME_OVER }) {
		gameOverAnimation.Update(timeElapsed);
		return;
	}
	else if (this->STATE == GAME_STATES{ GAME_READY }) {
		gameReadyAnimation.Update(timeElapsed);
		return;
	}
	else if (this->STATE == GAME_STATES{ RUNNING }) {
		this->IncreaseTimer(timeElapsed);

		this->obstacles.Update(timeElapsed);

		character.Update(timeElapsed);

		const int deletedCount = this->obstacles.EraseMarkedForDeletion();

		const int scoreDelta = (int)(timer * 10.0 * deletedCount);
		this->notch.IncreaseScore(scoreDelta);
		this->notch.Update(timeElapsed);

		if (!this->character.IsAlive()) {
			this->enterGameOverMode();
			return;
		}

		this->obstaclesFactory.Update(timeElapsed);
		this->obstaclesFactory.NewObstacleSpawn(
			&this->character,
			this->obstacles,
			this->playground,
			this->GetTimer(),
			this->obstacles.Size() == 0
		);

		this->bgScrollTreshold += timeElapsed * 32.0;
		this->bgScrollPos = (int)bgScrollTreshold % this->playground.Width();
	}
}

void Game::Draw(CDC& fDC) {
	if (this->STATE == GAME_STATES{ GAME_OVER }) {
		gameOverAnimation.GetFrame()->Draw(fDC.GetSafeHdc(), 0, 0);
	}
	else if (this->STATE == GAME_STATES{ INSTRUCTIONS }) {
		instructions.Draw(fDC.GetSafeHdc(), 0, 0);
	}
	else if (this->STATE == GAME_STATES{ GAME_READY }) {
		gameReadyAnimation.GetFrame()->Draw(fDC.GetSafeHdc(), 0, 0);
	}
	else if (this->STATE == GAME_STATES{ RUNNING }) {
		this->bg.Draw(
			fDC.GetSafeHdc(),
			-this->bgScrollPos,
			this->notch.GetHeight()
		);

		this->character.Draw(fDC);

		this->obstacles.Draw(fDC);

		this->notch.Draw(fDC);
	}
}

void Game::enterGameOverMode() {
	this->obstacles.Clear();
	this->player->SetHighestScore(this->notch.GetScore());
	this->setGameState(GAME_STATES{ GAME_OVER });
}

void Game::OnKeyDown(UINT key) {
	switch (key) {
	case VK_UP: {
		character.MoveUp();
		break;
	}
	case VK_DOWN: {
		character.MoveDown();
		break;
	}
	}
}

void Game::OnKeyUp(UINT key) {
	switch (key) {
	case VK_UP: {
		character.Stop();
		break;
	}
	case VK_DOWN: {
		character.Stop();
		break;
	}
	case VK_RETURN: {
		if (
			this->STATE == GAME_STATES{ GAME_READY } ||
			this->STATE == GAME_STATES{ GAME_OVER }
		) {
			this->Start();
		}
		break;
	}
	}
}

void Game::setGameState(const int& STATE) {
	this->STATE = STATE;
	this->publishGameState();
}

void Game::publishGameState() {
	std::list<IObserver<int>*>::iterator it = this->gameStateSubscribers.begin();
	while (it != this->gameStateSubscribers.end()) {
		(*it)->Renewal(this->STATE);
		it++;
	}
}

void Game::AddSubscriber(IObserver<int>* observer) {
	this->gameStateSubscribers.push_back(observer);
	observer->Renewal(this->STATE);
}

void Game::RemoveSubscriber(IObserver<int>* observer) {
	this->gameStateSubscribers.remove(observer);
}
