#pragma once

#include "pch.h"

struct Position {
	double x, y;
};

class GameObject {
	Position position;
	CRect existenceArea;
	CSize size;

public:

	const CRect& GetExistenceArea() const;

	void SetExistenceArea(const CRect& position);

	const CSize& GetSize() const;

	void SetSize(const CSize& size);

	const Position& GetPosition() const;

	void SetPosition(const Position& position);

	virtual void Update(const double& timeElapsed) = 0;
	virtual void Draw(CDC& fDC) = 0;
};

