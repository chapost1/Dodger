#pragma once

#include "Player.h"
#include "GameObject.h"

class GameNotch: public GameObject {
	int width;
	const int height;
	const int innerBorderHeight;
	const int playerIconEdgeLength;

	CImage background;
	Player* player;

	CImage* playerIcon;

	unsigned long long score;
	unsigned long long scoreToBeat;

	void updatePlayerIcon();

	int drawScore(CDC& fDC, int xPos);
	int drawPlayerName(CDC& fDC, int xPos);
	int drawScoreToBeat(CDC& fDC, int xPos);
	int drawText(CDC& fDC, const CString& text, int xPos, BOOL ltr);
public:
	GameNotch();
	virtual ~GameNotch();

	int GetHeight() const;

	void SetPlayer(Player* player);
	Player* GetPlayer() const;

	void SetScoreToBeat(const unsigned long long& scoreToBeat);
	unsigned long long GetScoreToBeat() const;

	void IncreaseScore(const int& delta);
	const unsigned long long& GetScore() const;

	void SetWidth(const int& px);

	void Start(Player* player, const unsigned long long& scoreToBeat);

	void Update(const double& timeElapsed) override;
	void Draw(CDC& fDC) override;
};

