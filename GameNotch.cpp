#include "pch.h"
#include "GameNotch.h"

GameNotch::GameNotch(): height(72), innerBorderHeight(2), playerIconEdgeLength(60) {
	this->player = nullptr;
	this->playerIcon = nullptr;
	this->background.Load(_T("./assets/backgrounds/notch-bg.jpg"));
	this->score = 0;
	this->scoreToBeat = 0;
	this->width = 0;
}

GameNotch::~GameNotch() {
	if (this->playerIcon != nullptr) {
		delete this->playerIcon;
		this->playerIcon = nullptr;
	}
}

int GameNotch::GetHeight() const {
	return this->height;
}

void GameNotch::SetPlayer(Player* player) {
	this->player = player;
	this->updatePlayerIcon();
}

Player* GameNotch::GetPlayer() const {
	return this->player;
}

void GameNotch::SetScoreToBeat(const unsigned long long& scoreToBeat) {
	this->scoreToBeat = scoreToBeat;
}

unsigned long long GameNotch::GetScoreToBeat() const {
	return this->scoreToBeat;
}

void GameNotch::IncreaseScore(const int& delta) {
	this->score += delta;
}

const unsigned long long& GameNotch::GetScore() const {
	return this->score;
}

void GameNotch::SetWidth(const int& px) {
	this->width = px;
}

void GameNotch::updatePlayerIcon() {
	if (this->player == nullptr) return;

	CString iconFileName;

	if (this->player->IsCustomIcon()) {
		iconFileName = this->player->GetIconFileName();
	}
	else {
		iconFileName = _T("default.png");
	}

	CString iconsDirectoryPath = _T("./assets/icons/");

	CString iconFilePath = iconsDirectoryPath + iconFileName;

	if (this->playerIcon != nullptr) {
		delete this->playerIcon;
		this->playerIcon = nullptr;
	}

	this->playerIcon = new CImage();

	this->playerIcon->Load(iconFilePath);
}

void GameNotch::Start(Player* player, const unsigned long long& scoreToBeat) {
	this->SetPlayer(player);
	this->SetScoreToBeat(scoreToBeat);
	this->score = 0;
}

void GameNotch::Update(const double& timeElapsed) {}

void GameNotch::Draw(CDC& fDC) {
	this->background.Draw(fDC.GetSafeHdc(), 0, 0);

	if (this->playerIcon == nullptr) return;

	this->playerIcon->Draw(
		fDC.GetSafeHdc(),
		this->playerIconEdgeLength / 2,
		(this->GetHeight() - this->innerBorderHeight - this->playerIconEdgeLength) / 2,
		this->playerIconEdgeLength,
		this->playerIconEdgeLength
	);

	const int margin = this->playerIconEdgeLength;

	int xPosLeft = this->drawPlayerName(fDC, margin * 2);

	int xPosRight = this->drawScoreToBeat(fDC, this->width - margin * 2);

	xPosLeft = this->drawScore(fDC, (xPosRight - xPosLeft) / 2 + xPosLeft - margin);
}

int GameNotch::drawPlayerName(CDC& fDC, int xPos) {
	return this->drawText(fDC, this->GetPlayer()->GetName(), xPos, TRUE);
}

int GameNotch::drawScore(CDC& fDC, int xPos) {
	CString score;
	score.Format(_T("Score: %llu"), this->GetScore());
	return this->drawText(fDC, score, xPos, TRUE);
}

int GameNotch::drawScoreToBeat(CDC& fDC, int xPos) {
	CString score;
	score.Format(_T("Score To Beat: %llu"), this->GetScoreToBeat());
	return this->drawText(fDC, score, xPos, FALSE);
}

int GameNotch::drawText(CDC& fDC, const CString& text, int xPos, BOOL ltr) {
	HFONT hTmp, hFont;
	int textHeight = 40;
	COLORREF oldTextColor = fDC.SetTextColor(RGB(255, 255, 255));
	int oldBackground = fDC.SetBkMode(TRANSPARENT);

	hFont = CreateFont(textHeight, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 2, 0, _T("SYSTEM_FIXED_FONT"));
	hTmp = (HFONT)fDC.SelectObject(hFont);

	const int textWidth = fDC.GetTextExtent(text).cx;

	const int textEndPos = ltr ? textWidth + xPos : xPos - textWidth;

	fDC.TextOutW(ltr ? xPos : textEndPos, (this->height - this->innerBorderHeight) / 2 - textHeight / 2, text);

	fDC.SetTextColor(oldTextColor);
	fDC.SelectObject(hTmp);
	DeleteObject(hFont);
	if (oldBackground) {
		fDC.SetBkMode(oldBackground);
	}
	return textEndPos;
}
