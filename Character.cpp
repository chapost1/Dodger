#include "pch.h"
#include "Character.h"
#include "GameConfig.h"
#include "VerticalMovingBehavior.h"

double Character::SPEED = OBJECTS_SPEED_FACTOR * 45;// max

Character::Character()
	: MovingObject()
{
	this->killed = FALSE;

	this->SetMovingBehavior(new VerticalMovingBehavior());

	this->SetSize(CSize{ (int)(229 * 0.65), (int)(176 * 0.65) });

	CString images[] = {
		_T("./assets/heroes/dragonite/dragonite-frame-1.png"),
		_T("./assets/heroes/dragonite/dragonite-frame-2.png"),
		_T("./assets/heroes/dragonite/dragonite-frame-3.png")
	};

	this->frames.Load(images, 3, 0.15);
}

void Character::MoveUp() {
	this->SetSpeed(-Character::SPEED);
}

void Character::MoveDown() {
	this->SetSpeed(Character::SPEED);
}

void Character::Stop() {
	this->SetSpeed(0);
}

BOOL Character::IsAlive() const {
	return !this->killed;
}

void Character::Kill() {
	this->killed = TRUE;
}

void Character::Update(const double& timeElapsed) {
	if (!this->detectWallsCollisions()) {
		MovingObject::Update(timeElapsed);
	}

	this->frames.Update(timeElapsed);
}

void Character::Draw(CDC& dc) {
	this->frames.GetFrame()->Draw(
		dc.GetSafeHdc(),
		(int)this->GetPosition().x,
		(int)this->GetPosition().y,
		this->GetSize().cx,
		this->GetSize().cy
	);
}

BOOL Character::detectWallsCollisions() const {
	int charTop = (int)this->GetPosition().y;
	int charBottom = (int)this->GetPosition().y + this->GetSize().cy;

	if (charTop <= this->GetExistenceArea().top && this->GetSpeed() < 0) {
		return TRUE;
	}

	if (this->GetExistenceArea().bottom <= charBottom && 0 < this->GetSpeed()) {
		return TRUE;
	}

	return FALSE;
}

void Character::Reset() {
	this->killed = FALSE;

	Position position;

	position.x = this->GetExistenceArea().left + 50;
	position.y = (this->GetExistenceArea().top + this->GetExistenceArea().bottom) / 2 - this->GetSize().cy;

	this->SetPosition(position);
}
