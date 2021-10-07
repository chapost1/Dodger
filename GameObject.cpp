#include "pch.h"
#include "GameObject.h"

const CRect& GameObject::GetExistenceArea() const {
	return this->existenceArea;
}

void GameObject::SetExistenceArea(const CRect& existenceArea) {
	this->existenceArea = existenceArea;
}

const CSize& GameObject::GetSize() const {
	return this->size;
}

void GameObject::SetSize(const CSize& size) {
	this->size = size;
}

const Position& GameObject::GetPosition() const {
	return this->position;
}

void GameObject::SetPosition(const Position& position) {
	this->position = position;
}
