#include "pch.h"
#include "Player.h"

IMPLEMENT_SERIAL(Player, CObject, 1)

void Player::Serialize(CArchive& ar) {
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		ar << name;
		ar << isCustomIcon;
		ar << iconFileName;
		ar << highestScore;
	}
	else { // Loading, not storing
		ar >> name;
		ar >> isCustomIcon;
		ar >> iconFileName;
		ar >> highestScore;
	}
}

Player::Player(const CString & name): highestScore(0), isCustomIcon(FALSE) {
	this->SetName(name);
}

Player::Player(const CString& name, const CString& iconFileName): highestScore(0), isCustomIcon(FALSE) {
	this->SetName(name);
	this->SetIconFileName(iconFileName);
}

CString Player::GetName() const {
	return this->name;
}

void Player::SetName(const CString& name) {
	this->name = name;
}

BOOL Player::IsCustomIcon() const {
	return this->isCustomIcon;
}

void Player::SetIsCustomIcon(const BOOL& isCustom) {
	this->isCustomIcon = isCustom;
}

CString Player::GetIconFileName() const {
	return this->iconFileName;
}

void Player::SetIconFileName(const CString& iconFileName) {
	this->iconFileName = iconFileName;
	this->SetIsCustomIcon(TRUE);
}

unsigned long long Player::GetHighestScore() const {
	return this->highestScore;
}

void Player::SetHighestScore(const unsigned long long& score) {
	if (this->GetHighestScore() < score) {
		this->highestScore = score;
	}
}
